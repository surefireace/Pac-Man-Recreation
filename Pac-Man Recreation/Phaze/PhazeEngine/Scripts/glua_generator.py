# Generate lua glue
import os

lua_conversions = dict()
lua_conversions['int'] = 'integer'
lua_conversions['const char *'] = 'string'
lua_conversions['unsigned long'] = 'integer'
lua_conversions['bool'] = 'boolean'
lua_conversions['float'] = 'number'

functions_generated = dict()

class Class:

    def __init__(self, name, file):
        self.functions = list()
        self.name = name
        self.file = file


class Function:

    def __init__(self, signature):
        self.signature = signature
        self.args = list()
        self.full_signature = ''
        self.glue_func_name = ''

        # Remove spaces AND tabs
        signature = signature.strip(' ')
        signature = signature.strip('\t')

        space_index = signature.find(' ')
        open_paren_index = signature.find('(')
        close_paren_index = signature.find(')')

        self.return_type = signature[0:space_index]
        self.name = signature[space_index + 1:open_paren_index]

        if open_paren_index + 1 == close_paren_index:
            # No arguments needed!
            return

        self.parse_args(signature[open_paren_index + 1:close_paren_index])
        # self.name =
        # self.args =
        # self.return_type =

    def parse_args(self, argString):
        comma_count = argString.count(',')

        if comma_count == 0:
            # 1 argument
            space_index = argString.rfind(' ')
            self.args.append(Argument(argString[:space_index], argString[space_index + 1:]))
            return

        arg_strings = argString.split(', ')
        for arg in arg_strings:
            space_index = arg.rfind(' ')
            self.args.append(Argument(arg[:space_index], arg[space_index + 1:]))


class Argument:

    def __init__(self, type, name):
        self.type = type
        self.name = name


def generate_glue(filename, classname):
    # Open the file and read it line by line
    current_class = Class(classname, filename)
    global functions_generated

    file = open(filename)
    file_lines = file.readlines()
    parse_function = False

    for line in file_lines:
        if parse_function:
            parse_function = False
            current_class.functions.append(Function(line))

        if 'LuaFunction' in line:
            parse_function = True

    file.close()

    if len(current_class.functions) == 0:
        return

    print('LuaFunction Generating: ' + current_class.name + '.gen.h')

    # Class parsed and current_class filled out completely
    header = open('..\\Source\\Generated\\' + current_class.name + '.gen.h', 'w')
    header.write('#include <lua.hpp>\n\n')

    for func in current_class.functions:
        func.glue_func_name = current_class.name + '_' + func.name + '_Glue'

        # Add the function name to our dictionary
        if current_class.name in functions_generated:
            functions_generated[current_class.name].append(func)
        else:
            functions_generated[current_class.name] = [func]

        func.full_signature = 'int ' + func.glue_func_name + '(lua_State* pState)'
        header.write(func.full_signature + ';\n')

    header.close()

    # Write the source (a little more complex)
    source = open('..\\Source\\Generated\\' + current_class.name + '.gen.cpp', 'w')
    source.write('#include "' + current_class.name + '.gen.h"\n\n')
    relative_path = os.path.relpath(filename, 'Source\\Generated\\')
    source.write('#include "' + relative_path + '"\n')

    for func in current_class.functions:
        source.write(func.full_signature + '\n{\n')

        arg_count = 0
        if func.args is None:
            arg_count = 1
        else:
            arg_count = len(func.args)
        # Get items from stack and convert them
        source.write('\tvoid* _pAddress = lua_touserdata(pState, -' + str(arg_count + 1) + ');\n')
        for i in range(arg_count):
            to_write = ''

            type = func.args[i].type
            to_write = '\t'
            to_write += type + ' _'
            to_write += func.args[i].name + ' = '

            # Check if primitive type
            if type in lua_conversions:
                to_write += '(' + type + ')lua_to' + lua_conversions[type]
                to_write += '(pState, -' + str(arg_count - i) + ');\n'
            else:
                to_write += 'static_cast<' + type + '>(lua_touserdata(pState, -' + str(arg_count - i) + '));\n'

            source.write(to_write)

        # Cast pointer to instance
        class_type = current_class.name
        source.write('\n\t' + class_type + '* _p' + class_type + ' = static_cast<' + class_type + '*>(_pAddress);\n')

        if func.return_type != 'void':
            source.write('\t' + func.return_type + ' result = ')
        else:
            source.write('\t')

        source.write('_p' + class_type + '->' + func.name + '(')

        # Pass arguments
        for i in range(arg_count):
            source.write('_' + func.args[i].name)

            if i != arg_count - 1:
                source.write(', ')

        source.write(');\n')

        # If there is a result, push it onto the stack
        if func.return_type != 'void':
            if func.return_type in lua_conversions:
                source.write('\tlua_push' + lua_conversions[func.return_type] + '(pState, result);\n')
            else:
                source.write('\tlua_pushlightuserdata(pState, result);\n')

        source.write('\n\tlua_pop(pState, ' + str(arg_count + 1) + ');\n')

        if func.return_type != 'void':
            source.write('\treturn 1;')
        else:
            source.write('\treturn 0;')

        source.write('\n}\n\n')

    source.close()


def write_lua_register():
    global functions_generated

    header = open('..\\Source\\Scripting\\RegisterLuaFunctions.h', 'w')
    header.write('//Generated File:\n')
    header.write('void RegisterLuaFunctions(struct lua_State* pState);')
    header.close()

    source = open('..\\Source\\Scripting\\RegisterLuaFunctions.cpp', 'w')
    source.write('#include "RegisterLuaFunctions.h"\n')
    source.write('#include <lua.hpp>\n\n')

    for file in functions_generated.keys():
        source.write('#include "..\\Generated\\' + file + '.gen.h"\n')

    source.write('void RegisterLuaFunctions(lua_State* pState)\n{\n')

    for key, value in functions_generated.items():
        # Create table
        source.write('\t// Function table for ' + key + '\n')
        source.write('\tlua_newtable(pState); // [+1]\n\n')

        for func in value:
            source.write('\tlua_pushcfunction(pState, ' + func.glue_func_name + '); // [+2]\n')
            source.write('\tlua_setfield(pState, -2, "' + func.name + '"); // [+1]\n')

        source.write('\n\tlua_setglobal(pState, "' + key + '"); // [+0]\n\n')

    source.write('}')
    source.close()

ignore_list = ['Defines.h', 'tinyxml2.h']

for root, dirs, files in os.walk('..\\Source\\'):
    for file in files:
        if '.h' in file and file not in ignore_list:
            path = os.path.join(root, file)
            generate_glue(path, file[:file.rfind('.')])

write_lua_register()

