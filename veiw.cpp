#include"type.h"


void outputAll(){
	
	struct jcb *p = NULL;
	p = (struct jcb*)malloc(jcb_size);
	p->arrival_time = 1;
	printf("��� %d", p->arrival_time); 
}

