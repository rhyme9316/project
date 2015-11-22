void swap(vector<int>& nums, int i, int j){
    int tmp = nums[i];
    nums[i] = nums[j];
    nums[j] = tmp;
    return;
}

void quicksort(vector<int>& nums, int begin, int end){
    if(begin >= end) return;
    
    int key = nums[begin];
    int i = begin, j = end;
    
    while(i < j){
        while(i < j && nums[j] > key){
            j--;
        }
        if(i < j) {
            swap(nums, i,j);
        }

        while(i < j && nums[i] <= key){
            i++;
        }
        if(i < j){
            swap(nums, i, j);
        }
    }
    
    quicksort(nums, begin, i-1);
    quicksort(nums, i+1, end);
    return;
}