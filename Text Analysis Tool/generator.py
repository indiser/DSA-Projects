import random
import string
import os

script_dir=os.path.dirname(os.path.abspath(__file__))

def generate_base_text(length):
    chars = string.ascii_lowercase + " "
    return ''.join(random.choices(chars, k=length))

def mutate_text(text, mutation_rate):
    mutated = []
    mutations_tracked = 0
    
    for char in text:
        if random.random() < mutation_rate:
            action = random.choice(['substitute', 'delete', 'insert'])
            mutations_tracked += 1
            
            if action == 'substitute':
                mutated.append(random.choice(string.ascii_lowercase))
            elif action == 'insert':
                mutated.append(random.choice(string.ascii_lowercase))
                mutated.append(char)
        else:
            mutated.append(char)
            
    print(f"Targeted Mutations Injected: ~{mutations_tracked}")
    return ''.join(mutated)

if __name__ == "__main__":
    # 1. Set your file size constraint here (start with 20,000 to test the waters)
    FILE_SIZE = 20000 
    MUTATION_RATE = 0.05 # 5% difference between the files
    
    print(f"Generating base text of {FILE_SIZE} characters...")
    base_text = generate_base_text(FILE_SIZE)
    
    print("Mutating text to create version 2...")
    mutated_text = mutate_text(base_text, MUTATION_RATE)
    
    with open(os.path.join(script_dir,"base_file.txt"), "w") as f1:
        f1.write(base_text)
        
    with open(os.path.join(script_dir,"mutated_file.txt"), "w") as f2:
        f2.write(mutated_text)
        
    print("Files 'base_file.txt' and 'mutated_file.txt' generated successfully.")