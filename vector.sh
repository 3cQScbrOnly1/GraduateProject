ANSWER=right_rate/final_answer.txt # 正确答案路径
WIN=9 # 词向量窗口数目
SIZE=100 # 词向量维度
CAL_TYPE=cos # 向量距离计算方式 
VECTOR_PARA=${SIZE}_${WIN}
WORDS_VECTOR=${HOME}/vector_model_b/${SIZE}/vector_model_b_${VECTOR_PARA}.bin # 词向量路径 
VECTOR_RESULT=vector_result_$VECTOR_PARA #缩略语还原答案
RIGHT_RATE=right_rate/cal_right_rate # 计算正确率的程序路径
RESTORE_VECTOR=vector/vector_restore # 缩略语还原程序
LINK_RESULT=link_result/link_result # 拼接测试语料 形成最后的还原语料
RESULT=restore/restore_vector_${CAL_TYPE}_${VECTOR_PARA} #最后还原的语料
INPUT=$PWD/input/needExtend_a # 测试语料
MARK=right_rate/mark # 缩略语类型

EXTEND_DIC=$PWD/dic/extendDic.txt # 扩展字典
COMBOS_DIC=$PWD/dic/combos.txt #  组合字典
STATIC_DIC=$PWD/dic/staticDic.txt # 静态字典

# 屏幕输出
echo $RESTORE_VECTOR $INPUT $WORDS_VECTOR $CAL_TYPE $EXTEND_DIC $COMBOS_DIC $STATIC_DIC $VECTOR_RESULT
# 获得缩略还原答案
$RESTORE_VECTOR $INPUT $WORDS_VECTOR $CAL_TYPE $EXTEND_DIC $COMBOS_DIC $STATIC_DIC $VECTOR_RESULT 
# 屏幕输出
echo $RIGHT_RATE $ANSWER $VECTOR_RESULT
# 算出还原答案的正确率
$RIGHT_RATE $ANSWER $VECTOR_RESULT $MARK
# 拼接出最后的还原语料
$LINK_RESULT $INPUT $VECTOR_RESULT > $RESULT

# 删除没有用的还原答案
rm $VECTOR_RESULT
