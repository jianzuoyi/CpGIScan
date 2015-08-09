import sys
import random

seq = 'AUGCCUAACGGUUGGAACUAG'
seq = 'ATCGGT'

def ShufflePreserveSinglet(source_sequence):
    random_seq = ''
    bases = list(source_sequence)
    while (len(bases) > 0):
        random_index = random.randint(0, len(bases)-1)
        random_seq += bases[random_index]
        #print('bases:', bases)
        #print(len(random_seq))
        del bases[random_index]
        
    return random_seq
    '''
    dest_sequence = ''
    random_checkout = []
    while (True):
        random_index = random.randint(0, len(source_sequence)-1)
        if random_index not in random_checkout:
            dest_sequence += source_sequence[random_index]
            random_checkout.append(random_index)
            if len(random_checkout) == len(source_sequence):
                break
    
    return dest_sequence    
    '''
def ShufflePreserveDoublet(source_sequence):
    A = []
    U = []
    G = []
    C = []

    i = 0
    lenght = len(source_sequence)
    for c in source_sequence:
        #print(c)
        if i == lenght - 1: break
        if c == 'A' or c == 'a':
            A.append(source_sequence[i+1])
        elif c == 'U' or c == 'u':
            U.append(source_sequence[i+1])
        elif c == 'G' or c == 'g':
            G.append(source_sequence[i+1])
        elif c == 'C' or c == 'c':
            C.append(source_sequence[i+1])
    
        i += 1
    
    print('A = ', A)
    print('U = ', U)
    print('G =', G)
    print('C =', C)
    A_checkout = []
    U_checkout = []
    G_checkout = []
    C_checkout = []
    
    seq = source_sequence[0]
    #while (True):
     #   c = seq[-1]
        
        
def main():
    seq = 'ATCGGT'
    seq = ShufflePreserveSinglet(seq)
    #dest_sequence = ShufflePreserveDoublet(seq)
    print('source  : %s'%seq)
    #print('shuffled: %s'%dest_sequence)
    
    
    
if __name__ == '__main__':
    main()