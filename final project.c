/* DS Project */
#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#define PI 3.141592653
#define R 6371000
#define recordCount 5232 
#define INFINITY 99999


double distance (double lat1, double lon1, double lat2, double lon2, double threshold)
{
	double a = pow(sin(((lat2-lat1)*PI/180)/2),2) + cos(lat1*PI/180)*cos(lat2*PI/180)*pow(sin(((lon2-lon1)*PI/180)/2),2);
	
	double c = 2 * atan2(sqrt(a),sqrt(1-a));
	
	double d = R * c ;

	if(d <= threshold || threshold==0.0)
      return d ;
    else
      return -1;
}

struct Hospital
{    
     char name[30] ;
	 double lat ;
	 double lon ;	
};

struct Hospdetails
{ 
  struct Hospital hospital ;
  double dis ;
  double time;
};

struct user
{
	
    char name[30];
    char bg[4];
    char issue[100];
    char attention[5];
    
} person ;

void sortByDistance(struct Hospdetails* nearby, int p, int r)
{
	if(p<r)
	{
		int q = partition(nearby, p,r);
		sortByDistance(nearby, p,q-1);
		sortByDistance(nearby, q+1,r);
	}
}

int partition(struct Hospdetails* nearby,int p , int r)
{
   int pivot = r ;
   int i  , j ; 
   j = p ;
   i = j - 1 ; 
   for( ; j < r ; j++)
   {
   	   if(nearby[j].dis < nearby[pivot].dis)
   	   {
   	     	i++;
   	     	swap(&nearby[i],&nearby[j]);
	   }
   }
   i++;
   swap(&nearby[i],&nearby[r]);
   return i ; 
}

void swap(struct Hospdetails* a , struct Hospdetails* b)
{
    struct Hospdetails temp = *a ;
    *a = * b ;
    *b = temp ;
}

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); 
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

struct Location
{
	 short ID ;
	 double lat ;
	 double lon;
};

struct link
{
	short s ;
	short t ;
	double len ;
	short directionflag;
};

struct AdjListNode
{
   short idx;
   struct Location info ;
   double weight;
   struct AdjListNode* next ;	
};

struct AdjList
{
	struct AdjListNode* HEAD;
};

struct MinHeapNode
{
   	short v ;
   	double weight;
}; 

struct MinHeap
{
	short capacity ;
	short size ;
	short* pos ;
	struct MinHeapNode** array;   
};

struct MinHeapNode* createMinHeapNode(int v, double weight)
{
   struct MinHeapNode* newnode = (struct MinHeapNode* )malloc(sizeof(struct MinHeapNode));
   newnode->v = v ;
   newnode->weight = weight ;
   return newnode;
}

struct MinHeap* createMinHeap(int capacity)
{
	struct MinHeap*  minHeap = (struct MinHeap* )malloc(sizeof(struct MinHeap));
	minHeap->capacity = capacity ;
	minHeap->size = 0 ;
	minHeap->pos = (short*)malloc(sizeof(short)*capacity);
	minHeap->array = (struct MinHeapNode**)malloc(sizeof(struct MinHeapNode*) * capacity);
	return minHeap;
}

void  swapMinHeapNode(struct MinHeapNode**a, struct MinHeapNode**b)
{
	struct MinHeapNode* t = *a;
	*a  =  *b ;
	*b = t ;
}

void minHeapify(struct MinHeap* minHeap, short idx)
{
	short smallest = idx ;
	short left , right ;
	
	left = 2*idx + 1 ;
	right = 2*idx + 2;
	
	if(left < minHeap->size && minHeap->array[left]->weight < minHeap->array[smallest]->weight)
	   smallest = left ;
	
	if(right < minHeap->size && minHeap->array[right]->weight < minHeap->array[smallest]->weight)
	    smallest = right ;
	    
	
	    
	if(smallest!=idx)
	{
		struct MinHeapNode* smallestNode = minHeap->array[smallest];
		struct MinHeapNode* idxNode = minHeap->array[idx];
		
		minHeap->pos[smallestNode->v] = idx;
		minHeap->pos[idxNode->v] =  smallest ;
		
		swapMinHeapNode(&minHeap->array[smallest],&minHeap->array[idx]);
		minHeapify(minHeap, smallest);
	}
}

short isEmpty(struct MinHeap* minHeap)
{
	return (minHeap->size==0);
}

struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
	if(isEmpty(minHeap))  return NULL;
	
	
	struct MinHeapNode* root = minHeap->array[0];
	struct MinHeapNode* lastNode = minHeap->array[minHeap->size-1];
	
	
	minHeap->array[0] = lastNode;


	
	minHeap->pos[root->v] = minHeap->size - 1 ;
	minHeap->pos[lastNode->v] = 0 ;
	
	
	--minHeap->size;
	minHeapify(minHeap,0);
	
	return root;
}

void decreaseKey(struct MinHeap* minHeap, short v, double dist)
{
	short i = minHeap->pos[v];
	
	minHeap->array[i]->weight = dist;
	
	
	while(i && minHeap->array[i]->weight < minHeap->array[(i-1)/2]->weight)
	{
		minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
		minHeap->pos[minHeap->array[(i-1)/2]->v] = i ;
		
		swapMinHeapNode(&minHeap->array[i],&minHeap->array[(i-1)/2]);
		i = (i-1)/2;
	}
}

int isInMinHeap(struct MinHeap* minHeap, short v)
{
	if(minHeap->pos[v] < minHeap->size)
	 return 1;
	return 0;
}

struct Graph
{
   short V ;
   struct AdjList* array ;	
}*graph;

double Dijkstra(short src,short dest,double ulat, double ulon,double hlat, double hlon, short HNo,  char* HName)
{
	short V = graph->V;
    double* dist = (double*)malloc(sizeof(double)*V);
    
    struct MinHeap* minHeap = createMinHeap(V);
    
    short i ; 
    for(i=0; i < V ; i++)
    {
    	dist[i] = INFINITY ;
    	minHeap->array[i] = createMinHeapNode(graph->array[i].HEAD->idx,dist[i]);
    	minHeap->pos[i] = i ;	 
	}
	
	short* parent ;
	parent = (short* )malloc(sizeof(parent) * V) ;
	
	parent[src] = -1  ;
	
 
    dist[src] = 0.0;
    
    
	decreaseKey(minHeap, src,dist[src]);
   
    minHeap->size = V;
    

    while(!isEmpty(minHeap))
    {
    	struct MinHeapNode* minHeapNode = extractMin(minHeap);
    	short u = minHeapNode->v;
        if (u==dest)   break;
    	struct AdjListNode* trav = graph->array[u].HEAD->next;
    	
    	
    	while(trav)
    	{
    		int v = trav->idx ;	
    		if(isInMinHeap(minHeap,v)  &&  dist[u]+trav->weight < dist[v])
    		{
    		
    			dist[v] = dist[u] + trav->weight;
    			decreaseKey(minHeap, v,dist[v]);
    			parent[v] =  u  ;
			}
		    
			trav = trav->next;
		}
	}
	
	
	  
      FILE* coordinateslist = fopen(concat("OutputFile ",concat(HName,".txt")),"w");
	
	  fprintf(coordinateslist,"%lf %lf\n",ulat,ulon);	
	
	
        printPath(src, dest,parent,coordinateslist);
    
        fprintf(coordinateslist,"%lf %lf",hlat,hlon);
        
        double minDistance = dist[dest];
        
        fclose(coordinateslist);
        
        
		return minDistance;
}

void printPath(short src,short dest, short* parent, FILE* coordinateslist)
{
	if(parent[dest]==-1)
	{
		fprintf(coordinateslist,"%lf %lf\n",graph->array[dest].HEAD->info.lat,graph->array[dest].HEAD->info.lon);
		return ; 
	}
	  
	printPath(src,parent[dest],parent, coordinateslist); 
	
	
	fprintf(coordinateslist,"%lf %lf\n",graph->array[dest].HEAD->info.lat,graph->array[dest].HEAD->info.lon);
}


void createGraph( int V)
{
	graph = (struct Graph*)malloc(sizeof(struct Graph));
	graph->V = V ;
	graph->array = (struct AdjList*)malloc(sizeof(struct AdjList)*V);
	int i ;
	for(i=0; i < V ; i++)
	 graph->array[i].HEAD = NULL;
}

void addNodetoarray(struct Location temp,short i)
{
	    struct AdjListNode* u = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
    	u->idx = i;
    	u->info = temp;
    	u->next = NULL;
    	u->weight = 0.0;

    	graph->array[u->idx].HEAD = u ;
}

int locate(double lat , double lon)
{
	short i , minid   ; 
	double min;
	for( i = 0 ; i<graph->V ; i++)
	{
	   if(i==0)  
	   {
	    min = distance(lat,lon,graph->array[i].HEAD->info.lat,graph->array[i].HEAD->info.lon,0.00);
	    minid = 0;
       }
	   else
	   {
	   	double d = distance(lat,lon,graph->array[i].HEAD->info.lat,graph->array[i].HEAD->info.lon,0.00 );
	   	if(d<min) 
	   	{
	   		min = d ;
	   		minid = i ;
		}
	   }
	}
	return minid;
}

struct AdjListNode* createAdjListNode(short index , double weight, struct Location L)
{
	struct AdjListNode* newnode = (struct AdjListNode* )malloc(sizeof(struct AdjListNode));
	newnode->idx =  index ;
	newnode->info = L ;
	newnode->next = NULL;
	newnode->weight = weight ;	
	return newnode;
}
void addEdge(short s , short t , struct Location L1 , struct Location L2 ,double weight, short directionflag)
{
	struct AdjListNode* newnode = createAdjListNode(t,weight,L2);
	
	struct AdjListNode* temp = graph->array[s].HEAD ; 
	newnode->next = temp->next ; 
	temp->next = newnode ; 
	
	if(directionflag==0)   return   ;
	
	else
	{
		newnode = createAdjListNode(s,weight,L1);
		temp = graph->array[t].HEAD ;
		newnode->next = temp->next ;
		temp->next =  newnode;
	}
}

int main()
{
	double lat,lon;           
	double mind;              // mind is the minimum radius of circle within which a hospital is found
	
	char anotherPatient ; 
	
	do{
	
	 printf("\n\n\n                            EMERGENCY SERVICES\n ");
	 printf("=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=");
	
	printf("\n\n Enter the users location  in terms of latitude and longitude\n" );   
	scanf("%lf%lf",&lat,&lon);          
	double dist , d ;
	
	FILE* fptr ;
	fptr = fopen("hospitals.DAT","rb+");   
	 
	 if(fptr==NULL)
	{
		printf("\nERROR : file did not open!");
		exit(1);
    }
    
    short int i = 0;
    
    FILE * fptrRIya;
    int ch = 1 ;

    printf("\n INPUT USER'S DATA  ");
    
	switch(ch)
    {
         case 1:
            fptrRIya = fopen("UserReport.txt", "w");
            if (fptrRIya == NULL)
            {
                printf("File does not exists \n");
                return;
            }
            fflush(stdin);
            printf("\n\nEnter your name: ");
            gets(person.name);
            fprintf(fptrRIya, "Name=  %s\n",person.name);
            fflush(stdin);
            printf("\n\nEnter your blood group: ");
            gets(person.bg);
            fprintf(fptrRIya, "Blood group=  %s\n",person.bg);
            fflush(stdin);
            printf("\n\nPlease write down the issue you are facing currently: ");
            gets(person.issue);
            fprintf(fptrRIya, "Issue=  %s\n",person.issue);
            fflush(stdin);
            printf("\n\nDo you require on-board assistance?");
            gets(person.attention);
            fprintf(fptrRIya, "Medical assistance required=  %s\n",person.attention);
            fclose(fptrRIya);
            break;
			
       }
    
    
    /* Stage 1 : Find all hospitals within a threshold radius and store them in array of structures */
    struct Hospital e ; 
    struct Hospdetails* nearby = (struct nearby*)calloc(10,sizeof(struct Hospdetails));

 	for(dist = 5000.0 ;  ; dist+=5000.0)
	{  
		rewind(fptr);
		mind = -1.00; 
		while(fread(&e,sizeof(e),1,fptr))
		{ 
		    
			 d  = distance(lat,lon,e.lat,e.lon,dist);

	
			if(d>0)
			{
		         nearby[i].hospital = e;	
				 nearby[i].dis = d ;
				 	         
		         i++;
		         if(i==1)
		          mind = d ;
		        else 
		          if(d<mind)
		            mind  = d ;
			}
		}
	  if (mind > 0 )
	     break;   
	     
	     
     }
     
     sortByDistance(nearby,0,i-1);
     
     struct Hospdetails *finalHospitals = (struct Hospdetails *)calloc(i,sizeof(struct Hospdetails));
          
    short k = 0 ;
    short j ; 
     
     for(j = 0 ; j < i ; j++)
     {
     	if(nearby[j].dis <= (mind+1000.0))
     	 finalHospitals[k++] = nearby[j];
	 }
	 free(nearby);
	 
     short useful = k ;
     
     while( k < i )  
	 {
	   free(finalHospitals+ k);
	   k++;
     }

	fclose(fptr); 
    
	FILE* links = fopen("Linksbinary.DAT","rb+");
    FILE* shivajinagar = fopen("shivajinagarbinary.DAT","rb+");
    
 
	struct Location L1, L2 ;

    createGraph(recordCount);
    i = 0 ; 
    
       while(fread(&L1,sizeof(L1),1,shivajinagar))
       {
		  addNodetoarray(L1,i);
		  i++;
       }
		
	struct link lnk ;
 
    while(fread(&lnk,sizeof(lnk),1,links) && lnk.s!=-1)
    {
    		
   	          fseek(shivajinagar,lnk.t*sizeof(struct Location),SEEK_SET);
    	  
    	   	  fread(&L2,sizeof(struct Location),1,shivajinagar);
              
			  fseek(shivajinagar,lnk.s*sizeof(struct Location),SEEK_SET);
			  
			  fread(&L1, sizeof(struct Location),1,shivajinagar);  
           
			  addEdge(lnk.s,lnk.t,L1,L2,lnk.len,lnk.directionflag);
	}
    fclose(shivajinagar);
    fclose(links);
	
	short useridx = locate(lat,lon);
		
	
	for(i = 0 ; i < useful ; i++)
	{
	  short Hospitalidx = locate(finalHospitals[i].hospital.lat,finalHospitals[i].hospital.lon);
	
	
	  d =  Dijkstra(Hospitalidx,useridx,lat,lon,finalHospitals[i].hospital.lat,finalHospitals[i].hospital.lon,i,finalHospitals[i].hospital.name);
	  
	  finalHospitals[i].dis = d/1000.0 ; 
	}
	
	sortByDistance(finalHospitals,0,useful-1);
	
	for(i=3; i < useful ; i++)
	{
		     int status = remove(concat("OutputFile ",concat(finalHospitals[i].hospital.name,".txt")));
		     
             if(status != 0 )   printf("\nError deleting the file!");
             
	}
	
	FILE* hospitalnames = fopen("Hospitalnames.txt","a+");
	
	if(hospitalnames==NULL){
		printf("\nFile didn't open !");
		exit(1);
	}
	
	printf("\nNearby hospitals found are :\n");
	for ( i = 0 ; i < 3 ; i++)
		printf("\n%d. %s",i+1,finalHospitals[i].hospital.name);
	printf("\n");
	
	for(i = 0 ; i < 3 ; i++)
		{
		  printf("\nMinimum distance from %s to user is %lf km.",finalHospitals[i].hospital.name, finalHospitals[i].dis );
	  
	     printf("\n\n\n");
		 fprintf(hospitalnames,"%s\n",finalHospitals[i].hospital.name);
	   }
	
	fclose(hospitalnames);
	
	
	printf("\nIs another patient available (y/n)?");
	fflush(stdin);
	scanf("%c",&anotherPatient);
	
	 for ( i = 0 ; i < 3 ; i++ )
     {
     	 int status = remove(concat("OutputFile ",concat(finalHospitals[i].hospital.name,".txt")));
         if(status != 0 )   printf("\nError deleting the file!");
         
         status = remove(concat("output ",concat(finalHospitals[i].hospital.name,".html")));
         if(status!=0)        printf("nError deleting HTML file!");
	 }
	 
	 int status = remove("Hospitalnames.txt");
     
     if(status != 0 )   printf("\nError deleting the file!");
     
     
     free(finalHospitals);
     
 } while(anotherPatient=='y'||anotherPatient=='Y');
   
	
    return 0 ;
  
}
    	


