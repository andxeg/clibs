"""
    This script generate test file with list of goods.
    FIELDS contains name of fields.
    TYPE - type of appropriate field -> 0 - numeric, 1 - string
    LENGTH_MIN - min length of field
    LENGTH_MAX - max length of field
"""

import sys
import string
from random import *

BOOL = 0
NUMBER = 1
STRING = 2

FIELDS = [
    "NAME",
    "COUNT_UNIT_NAME",
    "PRICE",
    "EATABLE",
    "LIQUID",
    "IMPORTED",
    "PACKED"
]

TYPES = [STRING, STRING, NUMBER, BOOL, BOOL, BOOL, BOOL]

LENGTH_MIN = [1, 1, 1, 1, 1, 1, 1]
LENGTH_MAX = [31, 4, 6, 1, 1, 1, 1]

def generate_random_num(min_digits, max_digits):
    min = 0 if min_digits == 1 else 10 ** (max_digits - 1)
    max = 10 ** max_digits - 1
    return randint(min, max)

def generate_random_string(min, max):
    # allchar = string.ascii_letters + string.punctuation + string.digits
    allchar = string.ascii_letters + string.digits + ' ' + '.'
    return "".join(choice(allchar) for x in range(randint(min, max)))

def generate_file_entry(file):
    result = []
    for i in range(len(FIELDS)):
        type = TYPES[i] 
        min = LENGTH_MIN[i]
        max = LENGTH_MAX[i]
        if type == STRING:
            result.append(generate_random_string(min, max))
        elif type == NUMBER:
            result.append(str(generate_random_num(min, max)))
        elif type == BOOL:
            result.append(str(randint(0, 1)))
        else:
            print("unknown field type")
    return result

def generate_file(filename, count):
    file = open(filename, "w")
    for i in range(count):
        entry = generate_file_entry(file)
        file.write('|'.join(entry) + '\n')
    file.close()

if __name__=="__main__":
    if len(sys.argv) != 3:
        print("error in input parameters")
        print("type %s <output filename> <number of goods>" % sys.argv[0])
        exit()
    
    output_filename = sys.argv[1]
    goods_count = int(sys.argv[2])
    generate_file(output_filename, goods_count)

