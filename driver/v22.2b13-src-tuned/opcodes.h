/*
	This file is automatically generated by make_func.
	do not make any manual changes to this file.
*/


/* operators */

#define F_POP_VALUE                    1
#define F_PUSH                         2
#define F_EFUN0                        3
#define F_EFUN1                        4
#define F_EFUN2                        5
#define F_EFUN3                        6
#define F_EFUNV                        7
#define F_SHORT_INT                    8
#define F_NUMBER                       9
#define F_REAL                         10
#define F_BYTE                         11
#define F_NBYTE                        12
#define F_STRING                       13
#define F_SHORT_STRING                 14
#define F_CONST0                       15
#define F_CONST1                       16
#define F_AGGREGATE                    17
#define F_AGGREGATE_ASSOC              18
#define F_BREAK_POINT                  19
#define F_BRANCH_WHEN_ZERO             20
#define F_BRANCH_WHEN_NON_ZERO         21
#define F_BRANCH                       22
#define F_BBRANCH_WHEN_ZERO            23
#define F_BBRANCH_WHEN_NON_ZERO        24
#define F_BBRANCH                      25
#define F_BRANCH_NE                    26
#define F_BRANCH_GE                    27
#define F_BRANCH_LE                    28
#define F_BRANCH_EQ                    29
#define F_BBRANCH_LT                   30
#define F_FOREACH                      31
#define F_NEXT_FOREACH                 32
#define F_EXIT_FOREACH                 33
#define F_LOOP_COND_LOCAL              34
#define F_LOOP_COND_NUMBER             35
#define F_LOOP_INCR                    36
#define F_WHILE_DEC                    37
#define F_LOR                          38
#define F_LAND                         39
#define F_CATCH                        40
#define F_END_CATCH                    41
#define F_TIME_EXPRESSION              42
#define F_END_TIME_EXPRESSION          43
#define F_SWITCH                       44
#define F_CALL_FUNCTION_BY_ADDRESS     45
#define F_CALL_INHERITED               46
#define F_RETURN                       47
#define F_RETURN_ZERO                  48
#define F_EQ                           49
#define F_NE                           50
#define F_LE                           51
#define F_LT                           52
#define F_GE                           53
#define F_GT                           54
#define F_INC                          55
#define F_DEC                          56
#define F_PRE_INC                      57
#define F_POST_INC                     58
#define F_PRE_DEC                      59
#define F_POST_DEC                     60
#define F_TRANSFER_LOCAL               61
#define F_MAKE_REF                     62
#define F_KILL_REFS                    63
#define F_LOCAL                        64
#define F_LOCAL_LVALUE                 65
#define F_REF                          66
#define F_REF_LVALUE                   67
#define F_GLOBAL                       68
#define F_GLOBAL_LVALUE                69
#define F_MEMBER                       70
#define F_MEMBER_LVALUE                71
#define F_INDEX                        72
#define F_INDEX_LVALUE                 73
#define F_RINDEX                       74
#define F_RINDEX_LVALUE                75
#define F_NN_RANGE                     76
#define F_NN_RANGE_LVALUE              77
#define F_RN_RANGE                     78
#define F_RN_RANGE_LVALUE              79
#define F_RR_RANGE                     80
#define F_RR_RANGE_LVALUE              81
#define F_NR_RANGE                     82
#define F_NR_RANGE_LVALUE              83
#define F_NE_RANGE                     84
#define F_RE_RANGE                     85
#define F_ADD_EQ                       86
#define F_SUB_EQ                       87
#define F_AND_EQ                       88
#define F_OR_EQ                        89
#define F_XOR_EQ                       90
#define F_LSH_EQ                       91
#define F_RSH_EQ                       92
#define F_MULT_EQ                      93
#define F_DIV_EQ                       94
#define F_MOD_EQ                       95
#define F_ASSIGN                       96
#define F_VOID_ADD_EQ                  97
#define F_VOID_ASSIGN                  98
#define F_VOID_ASSIGN_LOCAL            99
#define F_ADD                          100
#define F_SUBTRACT                     101
#define F_MULTIPLY                     102
#define F_DIVIDE                       103
#define F_MOD                          104
#define F_AND                          105
#define F_OR                           106
#define F_XOR                          107
#define F_LSH                          108
#define F_RSH                          109
#define F_NOT                          110
#define F_NEGATE                       111
#define F_COMPL                        112
#define F_FUNCTION_CONSTRUCTOR         113
#define F_SIMUL_EFUN                   114
#define F_SSCANF                       115
#define F_PARSE_COMMAND                116
#define F_NEW_CLASS                    117
#define F_NEW_EMPTY_CLASS              118
#define F_EXPAND_VARARGS               119

/* 1 arg efuns */
#define BASE 120

#define F__TO_INT                      120
#define F__TO_FLOAT                    121
#define F_THIS_PLAYER                  122
#define F_PREVIOUS_OBJECT              123
#define F_CALL_STACK                   124
#define F_SIZEOF                       125
#define F_DESTRUCT                     126
#define F_FILE_NAME                    127
#define F_CAPITALIZE                   128
#define F_RANDOM                       129
#define F_ALL_INVENTORY                130
#define F_DEEP_INVENTORY               131
#define F_FIRST_INVENTORY              132
#define F_NEXT_INVENTORY               133
#define F_MOVE_OBJECT                  134
#define F_COMMAND                      135
#define F_LIVING                       136
#define F_SET_LIVING_NAME              137
#define F_FIND_LIVING                  138
#define F_FIND_PLAYER                  139
#define F_NOTIFY_FAIL                  140
#define F_LOWER_CASE                   141
#define F_SAVE_VARIABLE                142
#define F_RESTORE_VARIABLE             143
#define F_WRITE                        144
#define F_SHOUT                        145
#define F_RECEIVE                      146
#define F_FIND_CALL_OUT                147
#define F_VALUES                       148
#define F_KEYS                         149
#define F_CLONEP                       150
#define F_INTP                         151
#define F_UNDEFINEDP                   152
#define F_FLOATP                       153
#define F_STRINGP                      154
#define F_VIRTUALP                     155
#define F_FUNCTIONP                    156
#define F_POINTERP                     157
#define F_OBJECTP                      158
#define F_CLASSP                       159
#define F_TYPEOF                       160
#define F_BUFFERP                      161
#define F_ALLOCATE_BUFFER              162
#define F_REPLACE_PROGRAM              163
#define F_CRC32                        164
#define F_FILE_SIZE                    165
#define F_MKDIR                        166
#define F_RM                           167
#define F_RMDIR                        168
#define F_CTIME                        169
#define F_LOCALTIME                    170
#define F_QUERY_IDLE                   171
#define F_QUERY_SNOOP                  172
#define F_QUERY_SNOOPING               173
#define F_SET_HEART_BEAT               174
#define F_QUERY_HEART_BEAT             175
#define F_SET_HIDE                     176
#define F_QUERY_SHADOWING              177
#define F_THROW                        178
#define F_DEEP_INHERIT_LIST            179
#define F_SHALLOW_INHERIT_LIST         180
#define F_MAPP                         181
#define F_INTERACTIVE                  182
#define F_IN_EDIT                      183
#define F_IN_INPUT                     184
#define F_USERP                        185
#define F_GET_CONFIG                   186
#define F_QUERY_PRIVS                  187
#define F_CHILDREN                     188
#define F_RELOAD_OBJECT                189
#define F_ERROR                        190
#define F_ED_CMD                       191
#define F_MUD_STATUS                   192
#define F_SET_EVAL_LIMIT               193
#define F_SET_DEBUG_LEVEL              194
#define F_CLEAR_DEBUG_LEVEL            195
#define F_FUNCTION_PROFILE             196
#define F_SWAP                         197
#define F_MKPASSWD                     198
#define F_REMOVE_SHADOW                199
#define F_COPY                         200
#define F_PLURALIZE                    201
#define F_FILE_LENGTH                  202
#define F_UPPER_CASE                   203
#define F_FETCH_VARIABLE               204
#define F_REMOVE_INTERACTIVE           205
#define F_DEBUG_MESSAGE                206
#define F_FUNCTION_OWNER               207
#define F_DB_CLOSE                     208
#define F_DB_COMMIT                    209
#define F_DB_ROLLBACK                  210
#define F_DB_AFFECTED_ROWS             211
#define F_DB_ERRNO                     212
#define F_DB_ERROR                     213
#define F_DB_INSERT_ID                 214
#define F_SOCKET_CLOSE                 215
#define F_SOCKET_ERROR                 216
#define F_REFS                         217
#define F_TRACEPREFIX                  218
#define F_TRACE                        219
#define F_COS                          220
#define F_SIN                          221
#define F_TAN                          222
#define F_ASIN                         223
#define F_ACOS                         224
#define F_ATAN                         225
#define F_SQRT                         226
#define F_LOG                          227
#define F_LOG10                        228
#define F_EXP                          229
#define F_FLOOR                        230
#define F_CEIL                         231
#define F_PARSE_REMOVE                 232

/* efuns */
#define ONEARG_MAX 233

#define F__CALL_OTHER                  233
#define F__EVALUATE                    234
#define F__THIS_OBJECT                 235
#define F__NEW                         236
#define F_BIND                         237
#define F_EXPLODE                      238
#define F_IMPLODE                      239
#define F_CALL_OUT                     240
#define F_MEMBER_ARRAY                 241
#define F_INPUT_TO                     242
#define F_ENVIRONMENT                  243
#define F_SAY                          244
#define F_TELL_ROOM                    245
#define F_PRESENT                      246
#define F_ADD_ACTION                   247
#define F_QUERY_VERB                   248
#define F_REMOVE_ACTION                249
#define F_COMMANDS                     250
#define F_DISABLE_COMMANDS             251
#define F_ENABLE_COMMANDS              252
#define F_LIVINGS                      253
#define F_REPLACE_STRING               254
#define F_RESTORE_OBJECT               255
#define F_SAVE_OBJECT                  256
#define F_USERS                        257
#define F_GET_DIR                      258
#define F_STRSRCH                      259
#define F_TELL_OBJECT                  260
#define F_MESSAGE                      261
#define F_FIND_OBJECT                  262
#define F_ALLOCATE_MAPPING             263
#define F_MAP_DELETE                   264
#define F_MATCH_PATH                   265
#define F_INHERITS                     266
#define F_REGEXP                       267
#define F_REG_ASSOC                    268
#define F_ALLOCATE                     269
#define F_CALL_OUT_INFO                270
#define F_READ_BUFFER                  271
#define F_WRITE_BUFFER                 272
#define F_WRITE_FILE                   273
#define F_RENAME                       274
#define F_WRITE_BYTES                  275
#define F_READ_BYTES                   276
#define F_READ_FILE                    277
#define F_CP                           278
#define F_LINK                         279
#define F_CLEAR_BIT                    280
#define F_TEST_BIT                     281
#define F_SET_BIT                      282
#define F_NEXT_BIT                     283
#define F_CRYPT                        284
#define F_OLDCRYPT                     285
#define F_EXEC                         286
#define F_FUNCTION_EXISTS              287
#define F_OBJECTS                      288
#define F_QUERY_HOST_NAME              289
#define F_QUERY_IP_NAME                290
#define F_QUERY_IP_NUMBER              291
#define F_SNOOP                        292
#define F_REMOVE_CALL_OUT              293
#define F_GENERATE_SOURCE              294
#define F_LPC_INFO                     295
#define F_SET_RESET                    296
#define F_SHADOW                       297
#define F_SORT_ARRAY                   298
#define F_TIME                         299
#define F_UNIQUE_ARRAY                 300
#define F_UNIQUE_MAPPING               301
#define F_PRINTF                       302
#define F_SPRINTF                      303
#define F_STAT                         304
#define F_MASTER                       305
#define F_MEMORY_INFO                  306
#define F_SET_PRIVS                    307
#define F_GET_CHAR                     308
#define F_UPTIME                       309
#define F_STRCMP                       310
#define F_RUSAGE                       311
#define F_FLUSH_MESSAGES               312
#define F_ED_START                     313
#define F_QUERY_ED_MODE                314
#define F_CACHE_STATS                  315
#define F_FILTER                       316
#define F_MAP                          317
#define F_MALLOC_STATUS                318
#define F_DUMPALLOBJ                   319
#define F_DUMP_FILE_DESCRIPTORS        320
#define F_QUERY_LOAD_AVERAGE           321
#define F_ORIGIN                       322
#define F_RECLAIM_OBJECTS              323
#define F_DEBUG_LEVELS                 324
#define F_OPCPROF                      325
#define F_RESOLVE                      326
#define F_SHUTDOWN                     327
#define F_QUERY_NOTIFY_FAIL            328
#define F_NAMED_LIVINGS                329
#define F_FUNCTIONS                    330
#define F_VARIABLES                    331
#define F_HEART_BEATS                  332
#define F_TERMINAL_COLOUR              333
#define F_REPLACEABLE                  334
#define F_PROGRAM_INFO                 335
#define F_STORE_VARIABLE               336
#define F_QUERY_IP_PORT                337
#define F_REPEAT_STRING                338
#define F_MEMORY_SUMMARY               339
#define F_QUERY_REPLACED_PROGRAM       340
#define F_NETWORK_STATS                341
#define F_DB_CONNECT                   342
#define F_DB_EXEC                      343
#define F_DB_FETCH                     344
#define F_DB_STATUS                    345
#define F_SOCKET_CREATE                346
#define F_SOCKET_BIND                  347
#define F_SOCKET_LISTEN                348
#define F_SOCKET_ACCEPT                349
#define F_SOCKET_CONNECT               350
#define F_SOCKET_WRITE                 351
#define F_SOCKET_RELEASE               352
#define F_SOCKET_ACQUIRE               353
#define F_SOCKET_ADDRESS               354
#define F_SOCKET_STATUS                355
#define F_DEBUG_INFO                   356
#define F_DESTRUCTED_OBJECTS           357
#define F_DUMP_PROG                    358
#define F_ID_MATRIX                    359
#define F_TRANSLATE                    360
#define F_SCALE                        361
#define F_ROTATE_X                     362
#define F_ROTATE_Y                     363
#define F_ROTATE_Z                     364
#define F_LOOKAT_ROTATE                365
#define F_POW                          366
#define F_PARSE_INIT                   367
#define F_PARSE_REFRESH                368
#define F_PARSE_SENTENCE               369
#define F_PARSE_ADD_RULE               370
#define F_PARSE_ADD_SYNONYM            371
#define F_PARSE_DUMP                   372
#define F_PARSE_MY_RULES               373

/* efuns */
#define NUM_OPCODES 373

