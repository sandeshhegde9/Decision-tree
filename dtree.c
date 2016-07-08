#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

struct data
{
	int s_no,class;
	char car,gender,size;
};
struct node
{
	int attribute,info;
	struct node *left,*right;
	int class;
};

typedef struct node *NODE;

NODE getNode();
NODE generateTree(struct data[],int);
void splitData(struct data[],int,struct data[],int*,struct data[],int*,int);
int getGini(struct data[],int);
void display(NODE);
void dotDump(NODE, FILE*);
void preorderDotDump (NODE, FILE*);
int assign(struct data,NODE);

int nodeCount=0;
FILE *outputFile;
main()
{
	int i,j,n,count=0,a;
	NODE root;
	FILE *pipe;
	scanf("%d",&n);
	struct data t[n];
	outputFile=fopen("bst.dot","w");
	for(i=0;i<n;i++)
	{
		scanf("%d ",&t[i].s_no);
		scanf("%c ",&t[i].gender);
		scanf("%c ",&t[i].car);
		scanf("%c ",&t[i].size);
		scanf("%d",&t[i].class);
	}
	root=generateTree(t,n);
//	display(root);
	dotDump(root,outputFile);
	for(i=0;i<n;i++)
	{
		a=assign(t[i],root);  		//calculating accuracy.
		if(a==t[i].class)
			count++;
	}
	a=((float)count/n)*100;
	printf("%d%c accurate.\n",a,'%');
//	system("dot -Tps bst.dot -o bst.ps");
//	system("display bst.ps");
}

int assign(struct data t,NODE root)  //assigns class to an unknown data set.
{
	while(root->class!=0&&root->class!=1)
	{
		switch(root->attribute)
		{
			case 1:	if(t.gender=='m')
					root=root->left;
				else
					root=root->right;
				break;
			case 2:	if(t.car=='s')
					root=root->left;
				else
					root=root->right;
				break;
			case 3:	if(t.size=='l')
					root=root->left;
				else
					root=root->right;
				break;
			default:break;
		}
	}
	return root->class;
}

NODE generateTree(struct data t[],int n)			//generates decision tree and returns root.
{
	NODE root=getNode();
	struct data t1[20],t2[20];
	int i,c0_count=0,c1_count=0,att,count1,count2;
	for(i=0;i<n;i++)
	{	
		if(t[i].class==1)					//counts the entries that belong to both classes.
			c1_count++;					//if either of the count is zero,node is assigned the other class and returned.
		else  						//(terminating condition)
			c0_count++;
	}
	if(c0_count==0)
	{
		root->class=1;
		return root;
	}
	else if(c1_count==0)
	{
		root->class=0;
		return root;
	}

	att=getGini(t,n);
	
	if(att==0)
	{
		if(c0_count>c1_count)				//if the attribute returned by 'getGni' function is 0, no further splitting is possible.
			root->class=0;				//so the node is assigned mejority of its paremt's class.
		else 							//else the entries are split based on the attribute.
			root->class=1;				//and recursively the left and right sub trees are constructed for reduced dataset.
		return root;
	}
	
	root->attribute=att;					
	count1=count2=0;
	splitData(t,n,t1,&count1,t2,&count2,att);
	if(count1==0)
	{
		root->left=getNode();
	 	if(c0_count>c1_count)
			root->left->class=0;
		else
			root->left->class=1;
	}
	else
		root->left=generateTree(t1,count1);	
	if(count2==0)
	{
		root->right=getNode();
	 	if(c0_count>c1_count)
			root->right->class=0;
		else
			root->right->class=1;
	}
	else
		root->right=generateTree(t2,count2);
	return root;
}

int getGini(struct data t[],int n)				//returns the attribute for which gini value is minumum.
{
	int i,count1,count2,att=0,c0_count,c1_count;
	struct data t1[20],t2[20];
	float gini1,gini2,gini3,min_gini=1,temp1,temp2;
	count1=0;
	count2=0;
	splitData(t,n,t1,&count1,t2,&count2,1);
	c0_count=c1_count=0;
	for(i=0;i<count1;i++)
	{
		if(t1[i].class==0)
			c0_count++;
		else
			c1_count++;
	}
	temp1=1-(c0_count/(float)count1)*(c0_count/(float)count1)-(c1_count/(float)count1)*(c1_count/(float)count1);
	c0_count=c1_count=0;
	for(i=0;i<count2;i++)
	{
		if(t2[i].class==0)
			c0_count++;
		else
			c1_count++;
	}
	temp2=1-(c0_count/(float)count2)*(c0_count/(float)count2)-(c1_count/(float)count2)*(c1_count/(float)count2);
	gini1=temp1*count1/n+temp2*count2/n;

	if(!(count1==0||count2==0))
	{
		min_gini=gini1;
		att=1;
	}
	
	count1=count2=0;
	splitData(t,n,t1,&count1,t2,&count2,2);
	c0_count=c1_count=0;
	for(i=0;i<count1;i++)
	{
		if(t1[i].class==0)
			c0_count++;
		else
			c1_count++;
	}
	temp1=1-(c0_count/(float)count1)*(c0_count/(float)count1)-(c1_count/(float)count1)*(c1_count/(float)count1);
	c0_count=c1_count=0;
	for(i=0;i<count2;i++)
	{
		if(t2[i].class==0)
			c0_count++;
		else
			c1_count++;
	}
	temp2=1-(c0_count/(float)count2)*(c0_count/(float)count2)-(c1_count/(float)count2)*(c1_count/(float)count2);
	gini2=temp1*count1/n+temp2*count2/n;

	if(!(count1==0||count2==0))
	{
		if(min_gini>gini2)
		{
			min_gini=gini2;
			att=2;
		}
	}

	count1=count2=0;
	splitData(t,n,t1,&count1,t2,&count2,3);
	c0_count=c1_count=0;
	for(i=0;i<count1;i++)
	{
		if(t1[i].class==0)
			c0_count++;
		else
			c1_count++;
	}
	temp1=1-(c0_count/(float)count1)*(c0_count/(float)count1)-(c1_count/(float)count1)*(c1_count/(float)count1);
	c0_count=c1_count=0;
	for(i=0;i<count2;i++)
	{
		if(t2[i].class==0)
			c0_count++;
		else
			c1_count++;
	}
	temp2=1-(c0_count/(float)count2)*(c0_count/(float)count2)-(c1_count/(float)count2)*(c1_count/(float)count2);
	gini3=temp1*count1/n+temp2*count2/n;

	if(!(count1==0||count2==0))
	{
		if(min_gini>gini3)
		{
			min_gini=gini3;
			att=3;
		}
	}
	return att;
}

void splitData(struct data t[],int n,struct data t1[],int *count1,struct data t2[],int *count2,int att)
{
	int i;
	switch(att)
	{
		case 1:	for(i=0;i<n;i++)
			{
				if(t[i].gender=='m')
				{
					t1[*count1]=t[i];
					(*count1)++;
				}						//splits the dats based on the attibute specified.
				else
				{
					t2[*count2]=t[i];
					(*count2)++;
				}
			}
			break;
		case 2:	for(i=0;i<n;i++)
			{
				if(t[i].car=='s')
				{
					t1[*count1]=t[i];
					(*count1)++;
				}
				else
				{
					t2[*count2]=t[i];
					(*count2)++;
				}
			}
			break;
		case 3:	for(i=0;i<n;i++)
			{
				if(t[i].size=='l')
				{
					t1[*count1]=t[i];
					(*count1)++;
				}
				else
				{
					t2[*count2]=t[i];
					(*count2)++;
				}
			}
			break;
		default:exit(0);
	}
}
NODE getNode()
{
	nodeCount++;
	NODE a=(NODE)malloc(sizeof(struct node));
	a->attribute=0;
	a->class=-1;
	a->left=NULL;
	a->right=NULL;
	a->info=nodeCount;
	return a;
}

void display(NODE root)
{
	if(root==NULL)
		return;
	printf("%d %d\n",root->class,root->attribute);
	display(root->left);
	display(root->right);
}
void preorderDotDump (NODE root, FILE* outputFile)
{
	if (root != NULL) 
	{
		if(root->class==0||root->class==1)
			fprintf (outputFile, "%d [label=%d,color=black];\n",root->info, root->class);
		else
			fprintf (outputFile, "%d [label=\"att %d\",color=black];\n",root->info, root->attribute);
		if (root->left != NULL) 
		         fprintf (outputFile, "%d -> %d ;\n", root->info, (root->left)->info);
		
        	if (root->right != NULL)
            		fprintf (outputFile, "%d -> %d;\n", root->info, root->right->info);
	//	if (root->pptr != NULL)
         //   		fprintf (outputFile, "%d -> %d [style = dashed];\n", root->info, (root->pptr)->info);
        	preorderDotDump (root->right, outputFile);
        	preorderDotDump (root->left, outputFile);
    	}
}

void dotDump(NODE root, FILE *outFile)
{
	fprintf (outFile, "digraph Decision {\n");
	
	preorderDotDump (root, outFile);
    	fprintf (outFile, "}\n");
}

