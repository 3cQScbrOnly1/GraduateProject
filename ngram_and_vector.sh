LM=$HOME/ngram_model_b/lm/my_b.arpa
ANSWER=right_rate/final_answer.txt
NGRAM_TYPE=binary
RIGHT_RATE=right_rate/cal_right_rate

WIN=9
SIZE=100
CAL_TYPE=cos
VECTOR_PARA=${SIZE}_${WIN}
WORDS_VECTOR=/home/yunan/vector_model_b/$SIZE/vector_model_b_${VECTOR_PARA}.bin

NGRAM_VECTOR=ngram_and_vector/ngram_and_vector_restore 
NBEST=2
N_V_RESULT=ngram_and_vector_${NGRAM_TYPE}_${CAL_TYPE}_${VECTOR_PARA}

RIGHT_RATE=right_rate/cal_right_rate
ANSWER=right_rate/final_answer.txt

INPUT=$HOME/load_exp/input/needExtend_a
LINK_RESULT=link_result/link_result
RESULT=restore/restore_ngram_and_vector_${NGRAM_TYPE}_${CAL_TYPE}_${VECTOR_PARA}

MARK=right_rate/mark # 缩略语类型

EXTEND_DIC=$HOME/load_exp/dic/extendDic.txt
COMBOS_DIC=$HOME/load_exp/dic/combos.txt
STATIC_DIC=$HOME/load_exp/dic/staticDic.txt

echo $NGRAM_VECTOR $INPUT $LM $WORDS_VECTOR $NGRAM_TYPE $CAL_TYPE $NBEST $EXTEND_DIC $COMBOS_DIC $STATIC_DIC $N_V_RESULT
$NGRAM_VECTOR $INPUT $LM $WORDS_VECTOR $NGRAM_TYPE $CAL_TYPE $NBEST $EXTEND_DIC $COMBOS_DIC $STATIC_DIC $N_V_RESULT

$RIGHT_RATE $ANSWER $N_V_RESULT $MARK
$LINK_RESULT $INPUT $N_V_RESULT > $RESULT

rm $N_V_RESULT
