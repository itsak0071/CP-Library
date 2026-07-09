
  struct  Node{
     int val;
       Node(){
         val=0;
       }
    Node( int p){
             val= p ;
    }

  };
 const int N= 1e6;
 Node seg[4*N+5];
 int arr[N];

  Node  combine(  Node a, Node b){
        Node res;
        res.val=a.val+ b.val;
        return res;
  }

 void  build( int s, int e ,int idx){
     if( s==e){
          seg[idx]= Node( arr[s]);
          return; 
     }
     int mid= ( s+e)/2;
     build(s,mid, 2*idx);
     build(mid+1 ,e, 2*idx+1);
     seg[idx]=combine( seg[2*idx], seg[2*idx+1]);

     


 }
 void update( int s ,int e, int  idx, int  update_index, int value){
          if( s==e){
               arr[s]=value;
               seg[idx]=Node(arr[s]);
               return ;
          }
           int mid= ( s+e)/2;
       if( update_index<=mid){
          update(s, mid, 2*idx, update_index,  value);
       }
      else{
         update(mid+1,e, 2*idx+1, update_index,  value);
      }
    
     seg[idx]=combine( seg[2*idx], seg[2*idx+1]);

     

 }
   Node query( int s , int e, int idx, int l  ,int r){
      if( s>r || e<l){
         return Node();
      }
      if(s>=l &&  e<=r ){
         return seg[idx];
      }
     int mid= (s+e)/2;
      Node  left=query( s, mid, 2*idx, l ,r);
      Node  right=query( mid+1, e, 2*idx+1, l ,r);
       return combine(left, right);



   }

  /* Haan bhai, 100% bilkul sahi! Is template ki sabse badi power hi yahi hai.

Ab tumhe kabhi bhi segment tree ka pura code (jaise build, update, query functions) har naye question ke liye scratch se sochne ya modify karne ki zarurat nahi padegi. Tumhara core structure ab fix aur bulletproof ho gaya hai.

Jab bhi koi naya question aaye, tumhe sirf 2 jagah par dimag lagana hai:

struct Node (State): Question ke hisab se yahan variables define karo aur unki default/identity value set karo (e.g., Sum ke liye 0, Min ke liye INF, Max ke liye -INF).

combine function (Logic): Dono child nodes (left aur right) ko merge karne ka logic yahan likho.

*/
