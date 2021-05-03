#include<stdio.h>
int main(){
	FILE *fp1=fopen("text.txt","r");
	FILE *fp2=fopen("output.txt","w");
	char c;
	while((c=fgetc(fp1))!=EOF){
		if(c=='/'){
			c=fgetc(fp1);
			if(c=='/'){
				while((c=fgetc(fp1))!='\n');
			}else if(c=='*'){
				while((c=fgetc(fp1))!='*') && (c=fgetc(fp1)!='/'));
			}else{
				fputc('/',fp2);
				fputc(c,fp2);
			}
		}else
			fputc(c,fp2);
	}
	fclose(fp1);
	fclose(fp2);
	return 0;
}
