#ifndef __VASL_LOGERMACRO_HPP__
#define __VASL_LOGERMACRO_HPP__

#define __VASLM_LOGER__ 	Vasl::log
#define __VASLM_INFLOGER__ 	Vasl::inflog
#define __VASLM_NORLOGER__ 	Vasl::norlog
#define __VASLM_WARLOGER__ 	Vasl::warlog
#define __VASLM_ERRLOGER__ 	Vasl::errlog

/* 详细日志 */
#define VASL_LOG(fmt, ...) 	\
							do { \
								__VASLM_LOGER__("\r\n\t~File:%s\r\n\t~Func:%s\r\n\t~Line:%d\r\n\t->" fmt, \
									__FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
							} while(0);
/* 详细info日志 */
#define VASL_INFLOG(fmt, ...) 	\
							do { \
								__VASLM_INFLOGER__("\r\n\t~File:%s\r\n\t~Func:%s\r\n\t~Line:%d\r\n\t->" fmt, \
									__FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
							} while(0);
/* 详细normal日志 */
#define VASL_NORLOG(fmt, ...) 	\
							do { \
								__VASLM_NORLOGER__("\r\n\t~File:%s\r\n\t~Func:%s\r\n\t~Line:%d\r\n\t->" fmt, \
									__FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
							} while(0);
/* 详细warning日志 */
#define VASL_WARLOG(fmt, ...) 	\
							do { \
								__VASLM_WARLOGER__("\r\n\t~File:%s\r\n\t~Func:%s\r\n\t~Line:%d\r\n\t->" fmt, \
									__FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
							} while(0);
/* 详细error日志 */
#define VASL_ERRLOG(fmt, ...) 	\
							do { \
								__VASLM_ERRLOGER__("\r\n\t~File:%s\r\n\t~Func:%s\r\n\t~Line:%d\r\n\t->" fmt, \
									__FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__); \
							} while(0);
/* 详细标志位日志 */
#define VASL_FLAGLOG		do { \
								__VASLM_LOGER__("\r\n\t~File:%s\r\n\t~Func:%s\r\n\t~Line:%d\r\n\t-> by:FlagLog\r\n", \
									__FILE__, __FUNCTION__, __LINE__); \
							} while(0);
#define VASL_STOPLOG        do { \
                                __VASLM_ERRLOGER__("\r\n\t~File:%s\r\n\t~Func:%s\r\n\t~Line:%d\r\n\t-> by:StopLog\r\n", \
                                    __FILE__, __FUNCTION__, __LINE__); \
                                    for(;;) {}  \
                            } while(0);
#define VASL_ASSERTLOG(bool, fmt, ...) 	\
                            do { \
                                if(!(bool)) { \
                                    __VASLM_ERRLOGER__("\r\n\t~File:%s\r\n\t~Func:%s\r\n\t~Line:%d\r\n\t-> by:AssertLog: %s\r\n", \
                                        __FILE__, __FUNCTION__, __LINE__, #bool); \
                                    for(;;) {} \
                                } \
                            } while(0);

#endif ///< __VASL_LOGERMACRO_HPP__
