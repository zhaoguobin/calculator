#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include <gtk/gtk.h>

#define YES 1
#define NO	0

GtkWidget *entry;
GtkWidget *frame;
GtkWidget *label;
gchar *title="<span foreground=\"red\">Bksp:退格</span> <span foreground=\"green\">CE:清除显示值</span> <span foreground=\"blue\">Clear:清除显示值和任何未完成的运算</span>";

guint Operate_flag=0;
guint Link_flag=YES;
guint Is_bksp=NO;
gdouble Resualt=0;

void clear_click();
void backspace_click();
void ce_click();

void flag_click(GtkButton *button,gpointer data);
void num_click(GtkButton *button,gpointer data);
void suan_click(GtkButton *button,gpointer data);
void deng_click();

void chufa(gdouble num);

GdkPixbuf *create_pixbuf(const gchar *filename);

int main(int argc,char *argv[])
{
	GtkWidget *window;
	GtkWidget *vbox;
	GtkWidget *table;
	GtkWidget *button;
	
	guint i,j,num;
	gchar *values[20]={"7","8","9",
					   "4","5","6",
					   "1","2","3",
					   "0",".","=",
					   "/","*","-","+",
					   "Bksp","CE","Clear","+/-"
				 };
	
	gtk_init(&argc,&argv);
	
	//窗体设置
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),"计算器");
	gtk_window_set_default_size(GTK_WINDOW(window),400,300);
	gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
	gtk_window_set_icon(GTK_WINDOW(window),create_pixbuf("calc.png"));
	gtk_container_set_border_width(GTK_CONTAINER(window),10);
	g_signal_connect(G_OBJECT(window),"destroy",G_CALLBACK(gtk_main_quit),NULL);
	
	vbox=gtk_vbox_new(FALSE,5);
	gtk_container_add(GTK_CONTAINER(window),vbox);
	
	table=gtk_table_new(6,4,TRUE);
	gtk_table_set_row_spacings(GTK_TABLE(table),10);
	gtk_table_set_col_spacings(GTK_TABLE(table),10);
	
	gtk_box_pack_start(GTK_BOX(vbox),table,TRUE,TRUE,0);
	
	entry=gtk_entry_new();
	gtk_entry_set_alignment(GTK_ENTRY(entry),1);
	gtk_table_attach_defaults(GTK_TABLE(table),entry,0,4,0,1);
	
	//数字0－9和小数点按钮
	num=0;
	for(j=2;j<6;j++)
	{
		for(i=0;i<3;i++)
		{
			button=gtk_button_new_with_label(values[num]);
			gtk_table_attach_defaults(GTK_TABLE(table),button,i,i+1,j,j+1);
			num++;
			
			g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(num_click),NULL);
			
			if(j==5 && i==1)
				i=3;
		}
	}
	
	//加、减、乘、除按钮
	num=12;
	for(j=2,i=3;j<6;j++,i--)
	{
		button=gtk_button_new_with_label(values[num]);
		gtk_table_attach_defaults(GTK_TABLE(table),button,3,4,j,j+1);
		num++;
			
		g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(suan_click),NULL);
	}
	
	//等号按钮
	button=gtk_button_new_with_label(values[11]);
	gtk_table_attach_defaults(GTK_TABLE(table),button,2,3,5,6);
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(deng_click),NULL);
	
	//Backspace按钮
	button=gtk_button_new_with_label(values[16]);
	gtk_table_attach_defaults(GTK_TABLE(table),button,0,1,1,2);
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(backspace_click),NULL);
	
	//CE按钮
	button=gtk_button_new_with_label(values[17]);
	gtk_table_attach_defaults(GTK_TABLE(table),button,1,2,1,2);
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(ce_click),NULL);
	
	//Clear按钮
	button=gtk_button_new_with_label(values[18]);
	gtk_table_attach_defaults(GTK_TABLE(table),button,2,3,1,2);	
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(clear_click),NULL);
	
	//+/-按钮
	button=gtk_button_new_with_label(values[19]);
	gtk_table_attach_defaults(GTK_TABLE(table),button,3,4,1,2);
	g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(flag_click),NULL);
	
	frame=gtk_frame_new("");
	gtk_box_pack_start(GTK_BOX(vbox),frame,TRUE,TRUE,0);
	label=gtk_label_new("");
	gtk_label_set_markup(GTK_LABEL(label),title);
	gtk_container_add(GTK_CONTAINER(frame),label);

	gtk_widget_show_all(window);
	
	gtk_main();
	
}

void num_click(GtkButton *button,gpointer data)
{
	Is_bksp=YES;
	gtk_label_set_markup(GTK_LABEL(label),title);
	
	gchar num[100]="";
	gchar num2[100]="";
	const gchar *text1;
	const gchar *text2;
	
	text1=gtk_entry_get_text(GTK_ENTRY(entry));
	text2=gtk_button_get_label(GTK_BUTTON(button));
	

	if(Link_flag==NO)
	{
		Link_flag=YES;
	}
	else
	{
		g_strlcat(num,text1,100);
	}
	g_strlcat(num,text2,100);
	
	gtk_entry_set_text(GTK_ENTRY(entry),num);

}

void suan_click(GtkButton *button,gpointer data)
{
	deng_click();
	
	const gchar *flag;
	
	flag=gtk_button_get_label(GTK_BUTTON(button));
	
	if(strcmp(flag,"+")==0)
		Operate_flag=1;
	else if(strcmp(flag,"-")==0)
			Operate_flag=2;
	else if(strcmp(flag,"*")==0)
			Operate_flag=3;
	else if(strcmp(flag,"/")==0)
			Operate_flag=4;
}

void deng_click()
{
	gtk_label_set_markup(GTK_LABEL(label),title);
	Is_bksp=NO;
	
	const gchar *text1;
	gdouble num,number;
	gchar *num1,num2[100]="",num3[100];
	gint pointer,fuhao;
	
	if(Link_flag==NO)
		Operate_flag=0;
	Link_flag=NO;
	
	text1=gtk_entry_get_text(GTK_ENTRY(entry));
	
	num=strtod(text1,NULL);

	switch(Operate_flag)
	{
		case 1:
			Resualt=Resualt+num;break;
		case 2:
			Resualt=Resualt-num;break;
		case 3:
			Resualt=Resualt*num;break;
		case 4:
			chufa(num);break;
		case 0:
			Resualt=num;break;
		}
	
	Operate_flag=0;
	
	num1=g_ascii_dtostr(num3,100,Resualt);

	g_strlcat(num2,num1,100);
	
	gtk_entry_set_text(GTK_ENTRY(entry),num2);
	
}

void clear_click()
{
	Operate_flag=0;
	Link_flag=YES;
	Is_bksp=NO;
	Resualt=0;
	gtk_label_set_markup(GTK_LABEL(label),title);
	gtk_entry_set_text(GTK_ENTRY(entry),"");
}

void backspace_click(GtkButton *button,gpointer data)
{
	gtk_label_set_markup(GTK_LABEL(label),title);
	
	const char *text1;
	char num[100]="";
	gint i=0;

	text1=gtk_entry_get_text(GTK_ENTRY(entry));
	strcpy(num,text1);
	
	if(Is_bksp==YES)
	{
		while(num[i])
			i++;
		i--;
		num[i]='\0';
	}
	
	gtk_entry_set_text(GTK_ENTRY(entry),num);
}

void ce_click(GtkButton *button,gpointer data)
{
	gtk_label_set_markup(GTK_LABEL(label),title);
	
	if(Is_bksp==YES)
		gtk_entry_set_text(GTK_ENTRY(entry),"");
}

void flag_click(GtkButton *button,gpointer data)
{
	gtk_label_set_markup(GTK_LABEL(label),title);
	
	const char *text1;
	char num[100]="";
	char num2[100]="";
	gint i=0;

	text1=gtk_entry_get_text(GTK_ENTRY(entry));
	strcpy(num,text1);
	
	if(Is_bksp==YES)
	{
		if(num[0]=='-')
			while(num[i])
			{
				num[i]=num[i+1];
				i++;
			}
		else
		{
			num2[0]='-';
			g_strlcat(num2,num,100);
			strcpy(num,num2);
		}
	}
	
	gtk_entry_set_text(GTK_ENTRY(entry),num);
}

void chufa(gdouble num)
{
	gchar *information;
	
	information="<span foreground=\"red\">错误！</span>除数不能为<span foreground=\"red\"><big>零</big></span>！";
	
	if(num==0)
	{
		gtk_label_set_markup(GTK_LABEL(label),information);
	}
	else
		Resualt=Resualt/num;
}

GdkPixbuf *create_pixbuf(const gchar *filename)
{
	GdkPixbuf *pixbuf;
	GError *error=NULL;
	
	pixbuf=gdk_pixbuf_new_from_file(filename,&error);
	
	if(!pixbuf) 
	{
      fprintf(stderr, "%s\n", error->message);
      g_error_free(error);
   	}
   	
   	return pixbuf;

}
