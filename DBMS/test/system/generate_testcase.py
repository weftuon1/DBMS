#!/usr/bin/env python
# coding: utf-8

import argparse
import os
import pandas as pd
import numpy as np
import random

def generate_user_table(name_list, size_user, iteration):
    # iteration repeat concat and sample to fit the size
    large_table = pd.concat([name_list]*(iteration+5), ignore_index=True)
    large_table = large_table.sample(frac=1).iloc[:size_user,:]
    # create user table
    large_table['id'] = large_table.index
    large_table['name'] = large_table.apply(lambda x: x['name'] + str(random.randint(0,2)), axis=1)
    large_table['email'] = large_table['name'] + '@nctu.edu'
    large_table['age'] =[random.randint(18, 90) for _ in range(size_user)]
    large_table['name'] = large_table['name'].apply(lambda x: '\"{}\"'.format(x))
    large_table['email'] = large_table['email'].apply(lambda x: '\"{}\"'.format(x))
    # change columns position
    cols_csv = ['id', 'name', 'email', 'age']
    large_table = large_table[cols_csv]
    return large_table

def generate_like_table(large_table, size_like):
    # create like table
    like_table = pd.DataFrame(columns=['id1','id2'])
    # id1 unique
    user_index_list = large_table['id'].tolist().copy()
    like_table['id1'] = random.sample(user_index_list, size_like)
    # id2 may not unique
    id2_idxs = np.random.randint(0, len(user_index_list), size = size_like)
    like_table['id2'] = [user_index_list[idx] for idx in id2_idxs]
    # avoid id1 and id2 the same value
    repeat_idx = like_table[like_table['id1']==like_table['id2']].index
    if len(repeat_idx) > 0:
        for idx in repeat_idx:
            sub_user_list = user_index_list.copy()
            sub_user_list.remove(like_table.loc[idx,'id1'])
            like_table.loc[idx,'id2'] = random.sample(sub_user_list, 1)[0]
    return like_table

# answer output format function
def answer_output(x):
    out_str = '('
    for col_n in range(len(x)):
        if col_n != 0:
            out_str = out_str+', '
        out_str = out_str+str(x[col_n])
    out_str = out_str+')'
    return out_str

def insert_string(x):
    str_out = "insert into"
    for col_n in range(len(x)):
        str_out = str_out +' '+ str(x[col_n])
    return str_out

def insert_table(table, table_name,cols_seq):
    # insert user table
    insert = table.copy()
    insert['table_name'] = table_name
    cols = ['table_name']
    cols.extend(cols_seq)
    insert = insert[cols].apply(insert_string, axis=1)
    return insert

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--user_num", default=100000, type=int, help="The number of user insert into table")
    args = parser.parse_args()

    base_path = os.path.dirname(__file__)
    name_list_dir = os.path.join(base_path, 'name.txt')
    testcase_dir = os.path.join(base_path, "testcases", "large")
    answer_dir = os.path.join(base_path, "answer", "large")
    if not os.path.isdir(testcase_dir):
        os.mkdir(testcase_dir)
    if not os.path.isdir(answer_dir):
        os.mkdir(answer_dir)

    size_user = args.user_num
    size_like = args.user_num // 2
    
    name_list = pd.read_csv(name_list_dir, sep="\n", header=None)
    name_list.columns=['name']

    # remove space in name 
    name_list['name'] = name_list['name'].str.replace(" ","")

    # calculate number of iteration to repeat
    iteration = int(size_user/len(name_list))

    # generate tables and insert query
    user_table = generate_user_table(name_list, size_user, iteration)
    like_table = generate_like_table(user_table, size_like)

    #user_table.to_csv('Final_project/user.csv', index=False)
    #like_table.to_csv('Final_project/like.csv', index=False)

    user_insert = insert_table(user_table, 'user',['id', 'name', 'email', 'age'])
    like_insert = insert_table(like_table, 'like',['id1', 'id2'])
   
    #---------------------------------------------------------------#
    # t1 -- select id, name from user offset <offset num> limit <limit num> 
    #---------------------------------------------------------------#
    # create insert txt (dataframe value to save txt)
    test_file_path = os.path.join(testcase_dir, 't1.txt')
    answer_file_path = os.path.join(answer_dir, 't1.txt')
    np.savetxt(test_file_path, user_insert.values, fmt='%s')

    # add query into testcase
    limit = args.user_num // 100 if args.user_num > 100 else 5
    offset = args.user_num // 10
    with open(test_file_path, 'a') as f:
        f.write("select id, name from user offset "+str(offset)+" limit "+str(limit)+"\n")
        f.write(".exit\n")

    # create answer
    # select id(0) and name(1)  from table
    answer = user_table.iloc[offset:offset+limit,[0,1]].apply(answer_output, axis=1)
    np.savetxt(answer_file_path, answer.values, fmt='%s')
    
    #---------------------------------------------------------------#
    # t2 -- select name, age from user where age <= {upper} and age >= {lower}
    #---------------------------------------------------------------#
    test_file_path = os.path.join(testcase_dir, 't2.txt')
    answer_file_path = os.path.join(answer_dir, 't2.txt')
    np.savetxt(test_file_path, user_insert.values, fmt='%s')

    # add query into testcase
    lower = random.randint(18, 30)
    upper = random.randint(lower, 50)
    with open(test_file_path, 'a') as f:
        f.write("select name, age from user where age <= {upper} and age >= {lower}\n".format(upper=upper, lower=lower))
        f.write(".exit\n")
    cols = ['name', 'age']
    answer = user_table.loc[(user_table['age']>=lower)&(user_table['age']<=upper),cols].apply(answer_output, axis=1)

    np.savetxt(answer_file_path, answer.values, fmt='%s')
    
    #---------------------------------------------------------------#
    # t3 -- select count(*) from user where age <= {upper} and age >= {lower}
    #---------------------------------------------------------------#    
    test_file_path = os.path.join(testcase_dir, 't3.txt')
    answer_file_path = os.path.join(answer_dir, 't3.txt')
    np.savetxt(test_file_path, user_insert.values, fmt='%s')

    # add query into testcase
    lower = random.randint(50, 70)
    upper = random.randint(lower, 90)
    with open(test_file_path, 'a') as f:
        f.write("select count(*) from user where age <= {upper} and age >= {lower}\n".format(upper=upper, lower=lower))
        f.write(".exit\n")
    answer = user_table.loc[(user_table['age']>=lower)&(user_table['age']<=upper),'age'].count()

    with open(answer_file_path, 'w') as f:
        f.write('(%d)\n' % answer)
    
    #--------------------------------------------------------------------------------------------#
    # t4 -- select count(*) from user join like on id = id1 where name = "{target_user}"
    #--------------------------------------------------------------------------------------------#  
    test_file_path = os.path.join(testcase_dir, 't4.txt')
    answer_file_path = os.path.join(answer_dir, 't4.txt')
    np.savetxt(test_file_path, user_insert.append(like_insert, ignore_index=True).values, fmt='%s')

    target_user = name_list.sample(n=1).iloc[0, 0]
    target_user = "{}{}".format(target_user, random.randint(0,2))
    with open(test_file_path, 'a') as f:
        f.write("select count(*) from user join like on id = id1 where name = \"{}\"\n".format(target_user))
        f.write(".exit\n")
    index = user_table.loc[(user_table['name']=='"{}"'.format(target_user))]
    if len(index)==0:
        answer = int(0)
    else:
        answer = pd.merge(like_table, index, how='inner', left_on=['id1'], right_on=['id'])
        answer = answer.shape[0]

    with open(answer_file_path, 'w') as f:
        f.write('(%d)\n' % answer)
    
    #--------------------------------------------------------------------------------------------#
    # t5 -- select count(*) from user join like on id = id2 where age < {target_age}
    #--------------------------------------------------------------------------------------------#
    test_file_path = os.path.join(testcase_dir, 't5.txt')
    answer_file_path = os.path.join(answer_dir, 't5.txt')
    np.savetxt(test_file_path, user_insert.append(like_insert, ignore_index=True).values, fmt='%s')

    target_age = random.randint(18, 90)
    with open(test_file_path, 'a') as f:
        f.write("select count(*) from user join like on id = id2 where age < {}\n".format(target_age))
        f.write(".exit\n")
    answer = pd.merge(user_table, like_table, how='inner', left_on=['id'],right_on=['id2'])
    answer = answer[answer['age']<target_age].loc[:,'name'].count()

    with open(answer_file_path, 'w') as f:
        f.write('(%d)\n' % answer)

if __name__ == "__main__":
    main()

