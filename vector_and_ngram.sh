LM=$HOME/ngram_model_b/lm/my_b.arpa
ANSWER=right_rate/final_answer.txt
NGRAM_TYPE=binary
RIGHT_RATE=right_rate/cal_right_rate

WIN=9
SIZE=100
CAL_TYPE=cos
VECTOR_PARA=${SIZE}_${WIN}
WORDS_VECTOR=/home/yunan/vector_model_b/$SIZE/vector_model_b_${VECTOR_PARA}.bin

VECTOR_NGRAM=vector_and_ngram/vector_and_ngram_restore 
NBEST=2
V_N_RESULT=vector_and_ngram_${CAL_TYPE}_${VECTOR_PARA}_${NGRAM_TYPE}

INPUT=$HOME/load_exp/input/needExtend_a
LINK_RESULT=link_result/link_result
RESULT=restore/restore_vector_and_ngram_${CAL_TYPE}_${VECTOR_PARA}_${NGRAM_TYPE}

EXTEND_DIC=$HOME/load_exp/dic/extendDic.txt
COMBOS_DIC=$HOME/load_exp/dic/combos.txt
STATIC_DIC=$HOME/load_exp/dic/staticDic.txt

echo $VECTOR_NGRAM $INPUT $LM $WORDS_VECTOR $NGRAM_TYPE $CAL_TYPE $NBEST $EXTEND_DIC $COMBOS_DIC $STATIC_DIC $V_N_RESULT
$VECTOR_NGRAM $INPUT $LM $WORDS_VECTOR $NGRAM_TYPE $CAL_TYPE $NBEST $EXTEND_DIC $COMBOS_DIC $STATIC_DIC $V_N_RESULT

$RIGHT_RATE $ANSWER $V_N_RESULT
$LINK_RESULT $INPUT $V_N_RESULT > $RESULT

rm $V_N_RESULT
