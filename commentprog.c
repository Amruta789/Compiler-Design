#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int main(int argc, char** argv){
	if(argc!=2){
		printf("One argument is expected\n");
		return 0;
	}
	const char* extension;
	extension = strrchr(argv[1],'.');
	if(!extension || extension == argv[1]){
		printf("File should be .c or .cc or .cpp\n");
		return 0;
	}
	if( !(strcmp(extension+1, "c")==0 || strcmp(extension+1, "cc")==0 || strcmp(extension+1, "cpp")==0) ){
		printf("File should be .c or .cc .cpp \n");
		return 0;
	}
	FILE *fp1=fopen(argv[1],"r");
	if(fp1==NULL){
		printf("File does not exist\n");
		return 0;
	}
	/* Tring a ling a Deteling
	 Abing a nong */
	char filename2[100];
	strcpy(filename2, "uncommented-");
	strcat(filename2,argv[1]);
	FILE *fp2=fopen(filename2,"w");
	// buffalo just added for testing.
	char c, buffalo;
	while((c=fgetc(fp1))!=EOF){
		if(c=='/'){
			c=fgetc(fp1);
			if(c=='/'){
				while((c=fgetc(fp1))!='\n');
				fputc('\n',fp2);
			}else if(c=='*'){
				while(1){
					if((c=fgetc(fp1))=='*'){
						if((c=fgetc(fp1))=='/'){
							break;
						}
					}
				}
			}else{
				fputc('/',fp2);
				fputc(c,fp2);
			}
		}else
			fputc(c,fp2);
	}
	buffalo = '8';
	fclose(fp1);
	fclose(fp2);
	return 0;
}
