#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<limits.h>
#include<math.h>

#define e 2.718

#define n 12
#define h 4
#define learning_rate 0.45
#define discount_rate 0.64

// structure for cache 
typedef struct cache{
	int val[n];
	int time[n];
	int freq[n];
}cache;

//strcture for history 
typedef struct history{
	int val[h];
	int time[h];
}history;

// initializing global cache structure 
cache *curr = NULL; 

// initializing global histories 
history *h_lru = NULL; 
history *h_lfu = NULL;

// initializing the weights for the two policies 
float w_lru=0.5;
float w_lfu=0.5;

int global_time=0;

// init function to initialize the cache and histories 
void init()
{
	for(int i=0;i<n;i++)
	{
		curr->val[i]=0;
		curr->time[i]=0;
		curr->freq[i]=0;
	}
	for(int i=0;i<h;i++)
	{
		h_lru->val[i]=0;
		h_lfu->val[i]=0;
	}
}

// utility function to print cache contents 
void print_cache()
{
	for(int i=0;i<n;i++)
		printf("%d ",curr->val[i]);
}

// utlity function to print history contents 
void print_history(history *temp)
{
	for(int i=0;i<h;i++)
		printf("%d ",temp->val[i]);
}

// returns a bool value indicating if cache is full or has empty slots 
bool is_memory_full(int *arr, int size)
{
	for(int i=0;i<size;i++)
		if(arr[i]==0)
			return false;
	return true;
}

// returns the index of a value in an array if it exists, else returns -1 
int in_array(int value, int *arr, int size)
{
	for(int i=0;i<size;i++)
	{
		if(arr[i]==value)
			return i;
	}
	return -1;
}

// deletes an element from cache using LRU policy 
int lru_deletion_cache(cache *temp)
{
	int max=0;
	for(int i=0;i<n;i++)
	{
		if(temp->time[i]>max)
			max=i;
	}
	int evicted_val=temp->val[max];
	temp->val[max]=0;
	return evicted_val;
}

// deletes an element from cache using LFU policy 
int lfu_deletion_cache(cache *temp)
{
	int min=INT_MAX;
	for(int i=0;i<n;i++)
	{
		if(temp->freq[i]<min)
			min=i;
	}
	int evicted_val = temp->val[min];
	temp->val[min]=0;
	return evicted_val;
}

// deletes an element from history using LRU policy 
void lru_deletion_hist(history *temp)
{
	int max=0;
	for(int i=0;i<h;i++)
	{
		if(temp->time[i]>max)
			max=i;
	}
	temp->val[max]=0;
}

// adds a value to history 
void add_to_hist(int value, history *temp)
{
	for(int i=0;i<h;i++)
	{
		if(temp->val[i]==0)
		{
			temp->val[i]=value;
			temp->time[i]=global_time;
			break;
		}
	}
}
 
// adds a value to cache
void add_to_cache(int value)
{
	for(int i=0;i<n;i++)
	{
		if(curr->val[i]==0)
		{
			curr->val[i]=value;
			curr->time[i]=global_time;
			curr->freq[i]=1;
			break;
		}
	}
}

// returns time spent by a page in history 
int time_spent(int page, history* temp)
{
	int t;
	int flag=0;
	for(int i=0;i<h;i++)
	{
		if(temp->val[i]==page)
		{
			flag=1;
			t=temp->time[i];
		}
	}
	int ret = global_time - t;
	if(flag==1)
		return ret;
	else
		return -1;
}

// function to update weights in accordance with regret minimization 
void update_weights(int page)
{
	if(in_array(page, h_lru->val, h)!=-1)
	{
		printf("%p\n", h_lru->val);
		int t = time_spent(page,h_lru);
		float r = pow(discount_rate,t);
		w_lfu *= pow(e,learning_rate*r);
	}
	else if(in_array(page,h_lfu->val,h)!=-1)
	{
		printf("Increasing lru\n");
		int t = time_spent(page,h_lfu);
		float r = pow(discount_rate,t);
		w_lru *= pow(e,learning_rate*r);
	}
	w_lru=w_lru/(w_lru+w_lfu);
	w_lfu = 1 - w_lru;
	printf("Updated weights are w_lru %lf, w_lfu %lf \n",w_lru, w_lfu);
}

// the main LeCaR implementation function 
void LeCaR(int *req, int size_query)
{
	// printf("Lecar called\n");
	for(int i=0;i<size_query;i++)
	{
		global_time+=1;
		if(in_array(req[i], curr->val, n)!=-1)
		{
			curr->freq[i]++;
			curr->time[i]=global_time;
		}
		else
		{
			int res = in_array(req[i], h_lru->val, h);
			if(res!=-1)
				h_lru->val[res]=0;

			res = in_array(req[i], h_lfu->val, h);
			if(res!=-1)
				h_lfu->val[res]=0;

			update_weights(req[i]);

			if(is_memory_full(curr->val,n))
			{
				int evicted_val;
				char action = w_lru>w_lfu?'r':'f';
				printf("Memory is full. Need to evict. \n");
				printf("Policy used %c\n",action);
				if(action=='r')
				{
					if(is_memory_full(h_lru->val,h))
						lru_deletion_hist(h_lru);
					evicted_val = lru_deletion_cache(curr);	
					add_to_hist(evicted_val, h_lru);	
				}
				else
				{
					if(is_memory_full(h_lfu->val,h))
						lru_deletion_hist(h_lfu);
					evicted_val = lfu_deletion_cache(curr);
					add_to_hist(evicted_val,h_lfu);
				}
			}
			add_to_cache(req[i]);
		}
		print_cache();
		printf("\t");
		print_history(h_lru);
		printf("\t");
		print_history(h_lfu);
		printf("\n");
	}
}

int main()
{
	curr = (cache *)malloc(sizeof(struct cache));
	h_lru = (history *)malloc(sizeof(struct history));
	h_lfu = (history *)malloc(sizeof(struct history));
	init();

	// TESTING WITH A SERIES OF 
	int req[13];
	for(int i=0;i<13;i++)
	{
		scanf("%d",&req[i]);
	}
	for(int i=0;i<n;i++)
	{
		curr->val[i]=req[i];
	}
	// h_lru->val[1] = 13; // Increases LFU wt.
	// h_lfu->val[1] = 13; // Increases LRU wt.
	LeCaR(req,13);
	return 0;
}