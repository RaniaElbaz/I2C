/*********************************************************************************/
/* Author    : Mostafa Rahal                                                     */
/* Version   : V01                                                               */
/* Date      : 19 Dec 2020                                                  	 */
/*********************************************************************************/

#ifndef ERROR_STATUS_H_
#define ERROR_STATUS_H_

typedef enum{
	OK,
	NOK,
	OUT_OF_RANGE,
	NULL_POINTER,
	TIME_OUT
}ErrorStatus_t;

#endif /* ERROR_STATUS_H_ */
