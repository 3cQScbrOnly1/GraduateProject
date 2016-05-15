ANSWER=right_rate/final_answer.txt
LM=$HOME/ngram_model_b/lm/my_b.arpa
NGRAM_TYPE=binary
RESULT=restore/restore_ngram_$NGRAM_TYPE
NGRAM_RESTORE=ngram/ngram_restore
NGRAM_RESULT=ngram_result_$NGRAM_TYPE
RIGHT_RATE=right_rate/cal_right_rate
LINK_RESULT=link_result/link_result
INPUT=$PWD/input/needExtend_a
RESULT=restore/restore_ngram_$NGRAM_TYPE
MARK=right_rate/mark # 缩略语类型

EXTEND_DIC=$PWD/dic/extendDic.txt
COMBOS_DIC=$PWD/dic/combos.txt
STATIC_DIC=$PWD/dic/staticDic.txt


echo $NGRAM_RESTORE $INPUT $LM $NGRAM_TYPE $EXTEND_DIC $COMBOS_DIC $STATIC_DIC $NGRAM_RESULT
$NGRAM_RESTORE $INPUT $LM $NGRAM_TYPE $EXTEND_DIC $COMBOS_DIC $STATIC_DIC $NGRAM_RESULT
echo $RIGHT_RATE $ANSWER $NGRAM_RESULT
$RIGHT_RATE $ANSWER $NGRAM_RESULT $MARK
$LINK_RESULT $INPUT $NGRAM_RESULT > $RESULT

rm $NGRAM_RESULT
