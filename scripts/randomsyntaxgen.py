import sys
import random

if len(sys.argv) <= 1:
    print("Please give an argument for the maximum program length.")
max_program_length = int(sys.argv[1])

specifiers = ["s/", "c/", "w/", "e/"]
block_starters = ["?/", "?>/"]
block_enders = ["/", "^/", "^^/", "^^^/"]

normal_tokens = ["#CHAR#", "#WORD#", "#TEXT#", "#INPUT_LENGTH#", "#STACK_LENGTH#",
                 ".", "!", "<", "<e", "+", "-", "o", "c", ">", ">e", "[]", "@[]",
                 # Technically not tokens, but some space should also be included with the program!
                 " ", "\t", "\n"]

# The current estimated length for the program.
current_length = 0

def add_token(tok: str, modify_current_length: bool = True):
    global current_length
    if modify_current_length:
        current_length += len(tok)
    print(tok, end="")


def create_normal_token():
    add_token(random.choice(normal_tokens))


def create_block_starter():
    global current_length
    block_ender_to_use = random.choice(block_enders)
    current_length += len(block_ender_to_use)
    add_token(random.choice(block_starters))
    create_insides()

    add_token(block_ender_to_use, modify_current_length = False)


def create_insides():
    while current_length < max_program_length and random.randint(0, 5) > 0:
        if random.randint(0, 10) == 10:
            create_block_starter()
        else:
            create_normal_token()
    

def create_specifier():
    global current_length
    add_token(random.choice(specifiers))
    current_length += 1 # For the block ender (/)
    create_insides()
    add_token("/", modify_current_length = False)

    
while current_length < max_program_length:
    create_specifier()