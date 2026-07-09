int arr[200001];
 int seg[4*200001];
   int combine(int a , int b){
       return   gcd(a,b);
   }
 
 void  build(int s ,int e, int index){
      if( s==e){
          seg[index]=arr[s];
          return ;
      }
       int mid=(s+e)/2;
       build(s,mid, 2*index);
       build(mid+1,e, 2*index+1);
       seg[index]=combine(seg[2*index], seg[2*index+1]);
 }
   void update( int s, int e, int index, int  index_to_update, int value){
       if( s==e){
          arr[index_to_update] = value;
         seg[index]= value;
         return;
       }
        int mid= (s+e)/2;
          if( mid<index_to_update){
               update(mid+1, e, 2*index+1, index_to_update, value);//riht side jao 
          }
          else{
              update(s, mid, 2*index, index_to_update, value);// left side update;

          }
          seg[index]=combine(seg[2*index], seg[2*index+1]);
   }
  int query( int s,   int e, int index ,int  l, int r){
       // nooverlap
    if( s>r || e<l){
         return 0;
    }
      if(l<=s && e<=r){
         return seg[index];
      }
        int mid= (s+e)/2;
        int leftcont=query(s ,mid, 2*index, l ,r);
        int rightcont=query( mid+1 ,e, 2*index+1, l ,r);
        return combine( leftcont, rightcont);


  }


int32_t main(){

 #ifndef ONLINE_JUDGE
     //for getting input from input.txt
     freopen("input1.txt","r",stdin);
     //for getting output to output.txt
     freopen("output1.txt","w",stdout);

 #endif
  int t ;
      int  n, q; cin>>n>>q;
      for(  int i=0; i< n ;i++){
           cin>>arr[i];
      }
      build(0 ,n-1, 1);
      while( q--){
          int type; cin>>type;
           if( type==1){//update;
              int index_to_update;
              int value;
              cin>>index_to_update>>value;
              update( 0 ,n-1, 1, index_to_update, value);

           }
           if( type==2){
                int l ,r;  cin>>l>>r;
                cout<<query(0, n-1, 1,  l ,r )<<endl;
           }

      }


    

     

     
     return 0;
         
 }
