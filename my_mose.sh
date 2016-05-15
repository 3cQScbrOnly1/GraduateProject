SHORT_FILE=for_mose/short
MOSE_TYPE=v5_add_space_model_b
MOSE_INI=$HOME/long_short/$MOSE_TYPE/working/train/model/moses.ini
MOSE=$HOME/mose/mosesdecoder-master/bin/moses
MOSE_ANSWER=for_mose/mose_answer
MOSE_TMP=for_mose/tmp
GET_SHORT=for_mose/get_short/get_short
INPUT=input/needExtend_a 
LINK=for_mose/link_short_and_my_result/link
STATIC_INPUT=for_mose/static_input
STATIC_RESTORE=$HOME/load_exp/for_mose/extend_by_static_dic/restore_by_static_dic
STATIC_DIC=dic/staticDic.txt
STATIC_RESULT=for_mose/static_result
RIGHT_RATE=for_mose/right_rate/right_rate_for_mose
ANSWER=$HOME/load_exp/right_rate/final_answer.txt
LINK_RESULT=link_result/link_result
RESULT=restore/mose_$MOSE_TYPE
MARK=right_rate/mark # 缩略语类型

$GET_SHORT $INPUT > $SHORT_FILE
cat $SHORT_FILE | $MOSE -f $MOSE_INI > $MOSE_TMP 
sed 's/ //g' $MOSE_TMP > $MOSE_ANSWER
$LINK $SHORT_FILE $MOSE_ANSWER > $STATIC_INPUT
$STATIC_RESTORE $STATIC_DIC $STATIC_INPUT > $STATIC_RESULT 
$RIGHT_RATE $ANSWER $STATIC_RESULT $SHORT_FILE $MARK
$LINK_RESULT $INPUT $STATIC_RESULT > $RESULT

#rm $SHORT_FILE
rm $MOSE_TMP
rm $STATIC_INPUT
rm $MOSE_ANSWER
rm $STATIC_RESULT
