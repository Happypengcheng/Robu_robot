#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//�˺�����������json���ݡ�ʹ���ݸ�ʽ�淶����

void MsgHandling(char msg_rcv[1024])
{
	size_t beginPos;  //���һ��json���ݵĿ�ʼλ��
	size_t lastPos;	  //���һ��json���ݵĽ���λ��
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