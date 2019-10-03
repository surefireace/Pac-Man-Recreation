import os

class_name = input('What class would you like to add?\n')
has_parent = input('Does it inherit from a class? (y/n)\n')
parent_name = ''
parent_dir = ''

if has_parent.lower() == 'y':
    parent_name = input('What class does it inherit from?\n')
    found_parent = False

    for root, dirs, files in os.walk('..\\Source'):
        for name in files:
            if name == parent_name + '.h':
                found_parent = True
                has_parent = True
                parent_dir = root
                print(parent_dir)
                quit(0)
                break

    if not found_parent:
        print('Could not find class {0}\n'.format(parent_name))
        quit(1)
else:
    has_parent = False

directory = input('Which directory should it live in?\n')

directory = '..\\Source\\' + directory
clean_dir = directory.strip('\r')
clean_class = class_name.strip('\r')
create_class = True

dir_exists = os.path.isdir(clean_dir)

if not dir_exists:
    print('{0} does not exist\n'.format(clean_dir))
    make_dir = input('Would you like to make it?  (y/n)\n')

    if make_dir.lower() == 'y':
        os.mkdir(clean_dir)
    else:
        print('Exiting...')
        quit(1)

# -- COPY HEADER --

# Open files
header_file_template = open('../Data/ClassTemplates/TemplateClassHeader.txt', 'r')
header_file_copy = open(clean_dir + '\\' + clean_class + '.h', 'w')
header_file_data = ''

# Copy data
if has_parent:
    header_file_data = header_file_template.read()
    header_file_data = header_file_data.replace('$parent_name', parent_name)
else:
    header_file_lines = header_file_template.readlines()

    for line in header_file_lines:
        if line.find('$parent_name') == -1:
            header_file_data += line

header_file_data = header_file_data.replace('$class_name', clean_class)

# Write data
header_file_copy.write(header_file_data)

# Close files
header_file_copy.close()
header_file_template.close()

# -- COPY SOURCE --

# Open files
source_file_template = open('../Data/ClassTemplates/TemplateClassCpp.txt', 'r')
source_file_copy = open(clean_dir + '\\' + clean_class + '.cpp', 'w')

# Copy data
source_file_data = source_file_template.read()
source_file_data = source_file_data.replace('$class_name', clean_class)

# Write data
source_file_copy.write(source_file_data)

# Close files
source_file_copy.close()
source_file_template.close()
