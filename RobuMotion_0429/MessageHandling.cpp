#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//此函数用来处理json数据。使数据格式规范化。

void MsgHandling(char msg_rcv[1024])
{
	size_t beginPos;  //最后一个json数据的开始位置
	size_t lastPos;	  //最后一个json数据的结束位置
	beginPos = lastPos = 0;
	for(int i = strlen(msg_rcv) - 1; i >= 0; i--){
		if(msg_rcv[i] == '}')
		{
			lastPos = i;
			if(lastPos != 0)
			{
				if(msg_rcv[i] == '{')
					beginPos = i;
			}
		}
		if(lastPos != 0 && beginPos != 0 && lastPos > beginPos)
				break;
	}
	for(i = 0; i<=lastPos-beginPos + 1; i++){
		msg_rcv[i] = msg_rcv[i+beginPos];
		if(i == lastPos-beginPos + 1)
			msg_rcv[i] = '\0';
	}
}