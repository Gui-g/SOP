final_queue = list()
insert_queue = list()

class Data:
    def __init__(self, bloco_inicial, num, op):
        self.bloco_inicial = bloco_inicial
        self.num = num
        self.op = op

    def __repr__(self):
        return str(self)

def insert(new_block):
    merge = 0
    if not final_queue:
        print("inserting: " + str(new_block.bloco_inicial))
        final_queue.append(new_block)
        return new_block
    else:
        for operation in insert_queue:
            print("bloco = " + str(operation.bloco_inicial))
            if operation.op == new_block.op and operation.num + new_block.num <= 64:
                if operation.bloco_inicial <= (new_block.bloco_inicial + new_block.num) and operation.bloco_inicial > new_block.bloco_inicial:
                    merge = 1
                    final_queue.remove(operation)
                    insert_queue.remove(operation)
                    operation = merge_block(new_block, operation)
                    insert(operation)
                elif new_block.bloco_inicial <= (operation.bloco_inicial + operation.num) and new_block.bloco_inicial > operation.bloco_inicial:
                    merge = 1
                    final_queue.remove(operation)
                    insert_queue.remove(operation)
                    operation = merge_block(new_block, operation)
                    insert(operation)
    if merge == 0:
        final_queue.append(new_block)
        return new_block
        


def merge_block(new_block, old_block):
    print("merging: " + str(new_block.bloco_inicial) + " " + str(old_block.bloco_inicial))
    #condições de merge:
    #(bloco inicial[old] + numero de blocos) == bloco inicial[new]
    #100 5, 105 5 => novo bloco inicia em 100, quantidade de blocos = old num + new num
    if old_block.bloco_inicial + old_block.num == new_block.bloco_inicial:
        print("case 1")
        old_block.num += new_block.num
    #o bloco novo está dentro do espaço do bloco antigo:
    #100 5, 101 2 => [101,103] dentro do intervalo [100,105]
    elif (old_block.bloco_inicial + old_block.num) > (new_block.bloco_inicial + new_block.num) and old_block.bloco_inicial < new_block.bloco_inicial:
        print("case 2")
        old_block.num = old_block.num
    #(bloco inicial[old] + numero de blocos) == (bloco inicial[new] + numero de blocos)
    #100 5, 95 10 => novo bloco inicial é o menor dos iniciais, num é do menor
    elif new_block.bloco_inicial + new_block.num == old_block.bloco_inicial + old_block.num:
        print("case 3")
        if new_block.bloco_inicial < old_block.bloco_inicial:
            old_block.bloco_inicial = new_block.bloco_inicial
            old_block.num = new_block.num
    #bloco inicial[new] > bloco inicial[old]
    #100 2, 101 5 => 100 6 (101-100 = 1 + 5 = 6)
    #100 5, 104 2 => 100 6 (104-100 = 4 + 2 = 6)
    #bloco inicial = bloco antigo
    #numero de blocos = |bloco1 - bloco2| + numero de blocos[new]
    elif new_block.bloco_inicial > old_block.bloco_inicial:
        print("case 4")
        old_block.num = (new_block.bloco_inicial - old_block.bloco_inicial) + new_block.num
    #bloco inicial[old] > bloco inicial[new]
    #104 2, 100 5 => 100 6 (104-100 = 4 + 2 = 6)
    #101 5, 100 2 => 100 6 (101-100 = 1 + 5 = 6)
    #bloco inivial = bloco antigo
    #numero de blocos = |bloco1 - bloco2| + numero de blocos[old]
    elif old_block.bloco_inicial > new_block.bloco_inicial:
        print("case 6")
        old_block.num = (old_block.bloco_inicial - new_block.bloco_inicial) + old_block.num
        old_block.bloco_inicial = new_block.bloco_inicial
    #os dois blocos iniciam no mesmo bloco
    #100 == 100 => num = o maior deles
    elif old_block.bloco_inicial == new_block.bloco_inicial:
        print("case 7")
        if old_block.num < new_block.num:
            old_block.num = new_block.num

    print("pós merge: " + str(old_block.bloco_inicial) + " " + str(old_block.num))    
    return old_block


if __name__ == "__main__":
    while(1):
        bloco_inicial = int(input())

        if bloco_inicial == -1:
            break
        else:
            num = int(input())
            op = input()
            entry = Data(bloco_inicial, num, op)
            new = insert(entry)
            insert_queue.append(new)
            print([element.bloco_inicial for element in final_queue])

    final_queue.sort(key=lambda x: x.bloco_inicial, reverse=True)
    print("Fila:")
    for operation in final_queue:
        print(str(operation.bloco_inicial) + " " + str(operation.num) + " " + str(operation.op))