#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int value;
	struct node *left;
	struct node *right;
}node;

node *root=NULL;
int ch=0;

void insert(int data,node **leaf);
void preOrder(node *leaf,int order[][2]);
void freeList(node *head);

int main(void) {
	FILE *fin=fopen("input.txt","r");
	FILE *fout=fopen("output.txt","w");
	int t1node,t2node;
	int cnt1=0,cnt2=0;
	int i,j,parent,child;

	// recieve input file
	fscanf(fin,"%d\n",&t1node);	// number of nodes in T1
	int tree1[t1node-1][2];
	
	for(i=0;i<t1node-1;i++) {
		fscanf(fin,"%d %d\n",&parent,&child);
		tree1[i][0]=parent;
		tree1[i][1]=child;
	}
	fscanf(fin,"%d\n",&t2node);	// number of nodes in T2
	
	int tree2[t2node-1][2];
	for(i=0;i<t2node-1;i++) {
		fscanf(fin,"%d %d\n",&parent,&child);
		tree2[i][0]=parent;
		tree2[i][1]=child;
	}
	
	int nd1[t1node],nd2[t2node];	//array including value of all nodes
	nd1[0]=tree1[0][0];
	nd2[0]=tree2[0][0];
	for(i=0;i<t1node-1;i++) {
		nd1[i+1]=tree1[i][1];
	}
	for(i=0;i<t2node-1;i++) {
		nd2[i+1]=tree2[i][1];
	}
	
	fclose(fin); //close input file
	
	// list containing value of nodes & connects in T3
	int bst1[t1node+t2node-1][2], bst2[t1node+t2node-1][2];
	node *treeNode1,*treeNode2;
	
	// create merged tree (base:T1, base:T2) = T3_1
	for(i=0;i<t1node;i++) {
		insert(nd1[i],&root);	// create T1
	}
	for(i=0;i<t2node;i++) {
		insert(nd2[i],&root);	// merge nodes of T2 into T1
	}
	treeNode1=root;
	preOrder(treeNode1,bst1);

	// create merged tree (base:T2, base:T1) = T3_2
	root=NULL;
	ch=0;
	for(i=0;i<t2node;i++) {
		insert(nd2[i],&root);	// create T2
	}
	for(i=0;i<t1node;i++) {
		insert(nd1[i],&root);	// merge nodes of T1 into T2
	}
	treeNode2=root;
	preOrder(treeNode2,bst2);
	
	for(i=0;i<t2node-1;i++) {
		for(j=0;j<t1node+t2node-1;j++) {
			// find if same connections in T2 exist in T3_1
			if(tree2[i][0]==bst1[j][0] && tree2[i][1]==bst1[j][1]) break;
		}
		if(j==t1node+t2node-1) cnt1++;	// no same connection in T3_1 -> (number of deleted edges)++
	}

	for(i=0;i<t1node-1;i++) {
		for(j=0;j<t1node+t2node-1;j++) {
			// find if same connections in T1 exist in T3_2
			if(tree1[i][0]==bst2[j][0] && tree1[i][1]==bst2[j][1]) break;
		}
		if(j==t1node+t2node-1) cnt2++;	// no same connection in T3_2 -> (number of deleted edges)++
	}

	if(cnt1<=cnt2) {	// print out T3_1
		fprintf(fout,"%d\n",treeNode1->value);
		for(i=0;i<t1node+t2node-1;i++) {
			fprintf(fout,"%d %d\n",bst1[i][0],bst1[i][1]);
		}
		fprintf(fout,"%d\n",cnt1+1);	// number of added edges
		fprintf(fout,"%d\n",cnt1);		// number of deleted edges
	}
	else {				// print out T3_2
		fprintf(fout,"%d\n",treeNode2->value);
		for(i=0;i<t1node+t2node-1;i++) {
			fprintf(fout,"%d %d\n",bst2[i][0],bst2[i][1]);
		}
		fprintf(fout,"%d\n",cnt2+1);	// number of added edges
		fprintf(fout,"%d\n",cnt2);		// number of deleted edges
	}

	// free nodes in T3_1, T3_2
	freeList(treeNode1);
	freeList(treeNode2);
	
	// close output file
	fclose(fout);

	return 0;
}

void insert(int data,node **leaf) {
	if(*leaf==NULL) {	// insert leaf into BST
		*leaf=(node *)malloc(sizeof(node));
		(*leaf)->value=data;	// put in value
		(*leaf)->left=NULL;
		(*leaf)->right=NULL;
	}
	else if(data<(*leaf)->value) {		// go down left
		insert(data,&(*leaf)->left);
	}
	else if(data>(*leaf)->value) {		// go down right
		insert(data,&(*leaf)->right);
	}
}

void preOrder(node *leaf,int order[][2]) {
	node *temp1,*temp2;
	
	if(leaf!=NULL) {
		if(leaf->left!=NULL) {
			order[ch][0]=leaf->value;
			temp1=leaf->left;
			order[ch++][1]=temp1->value;
		}
		if(leaf->right!=NULL) {
			order[ch][0]=leaf->value;
			temp2=leaf->right;
			order[ch++][1]=temp2->value;
		}
		preOrder(leaf->left,order);
		preOrder(leaf->right,order);
	}
	else return;
}

void freeList(node *head) {
	if(head!=NULL) {
		freeList(head->left);
		freeList(head->right);
		free(head);
	}
	else return;
}
