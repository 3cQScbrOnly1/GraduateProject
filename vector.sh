ANSWER=right_rate/final_answer.txt
WIN=9
SIZE=100
CAL_TYPE=cos
VECTOR_PARA=${SIZE}_${WIN}
WORDS_VECTOR=${HOME}/vector_model_b/${SIZE}/vector_model_b_${VECTOR_PARA}.bin
VECTOR_RESULT=vector_result_$VECTOR_PARA
RIGHT_RATE=right_rate/cal_right_rate
RESTORE_VECTOR=vector/vector_restore
RIGHT_RATE=right_rate/cal_right_rate
LINK_RESULT=link_result/link_result
RESULT=restore/restore_vector_${CAL_TYPE}_${VECTOR_PARA}
INPUT=$HOME/load_exp/input/needExtend_a

EXTEND_DIC=$HOME/load_exp/dic/extendDic.txt
COMBOS_DIC=$HOME/load_exp/dic/combos.txt
STATIC_DIC=$HOME/load_exp/dic/staticDic.txt

echo $RESTORE_VECTOR $INPUT $WORDS_VECTOR $CAL_TYPE $EXTEND_DIC $COMBOS_DIC $STATIC_DIC $VECTOR_RESULT
$RESTORE_VECTOR $INPUT $WORDS_VECTOR $CAL_TYPE $EXTEND_DIC $COMBOS_DIC $STATIC_DIC $VECTOR_RESULT
echo $RIGHT_RATE $ANSWER $VECTOR_RESULT
$RIGHT_RATE $ANSWER $VECTOR_RESULT
$LINK_RESULT $INPUT $VECTOR_RESULT > $RESULT


rm $VECTOR_RESULT
