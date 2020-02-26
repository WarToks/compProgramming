# パスの設定 
path_to_input_file = 'input/input01.txt'
    

# 前回の内容の消去
with open(path_to_input_file, mode = 'w'):
    pass

with open(path_to_input_file, mode='a') as the_file: # 追記
    n = 2 
    the_file.write(str(n) + '\n')
    out = ['z' for _ in range(n)]
    the_file.write('\n'.join(out))