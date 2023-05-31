# 概念
就是对于区间的一种动态规划，对于某个区间，它的合并方式可能有很多种，我们需要去枚举所有的方式，通常是去枚举区间的分割点，找到最优的方式(一般是找最少消耗)。

例如：对于区间【i，j】，它的合并方式有很多种，可以是【i，i+1】和【i+2，j】也可以是【i，k】和【k+1，j】（其中i <= k < j）
![](../%E5%9B%BE/%E5%8C%BA%E9%97%B4dp.png)

在合并区间时，一般会有消耗（根据题意去计算），状态转移方程就可以表示成：
dp[i][j] = min(dp[i][j], dp[i,k] + dp[k+1][j] + 合并区间的消耗 ) 


# lc1130. 叶值的最小代价生成树
> 给你一个正整数数组 arr，考虑所有满足以下条件的二叉树：  

> - 每个节点都有 0 个或是 2 个子节点。  
> - 数组 arr 中的值与树的中序遍历中每个叶节点的值一一对应。  
> - 每个非叶节点的值等于其左子树和右子树中叶节点的最大值的乘积。  

在所有这样的二叉树中，返回每个非叶节点的值的最小可能总和。这个和的值是一个 32 位整数。  
如果一个节点有 0 个子节点，那么该节点为叶节点。  


```cpp
    int mctFromLeafValues(vector<int>& arr) {
        int n = arr.size();
        vector<int>dp(n, 0);
        vector<int>max_num(n, 0);
        dp[0] = 0; max_num[0] = arr[0];
        dp[1] = arr[0] * arr[1]; max_num[1] = max(arr[0] , arr[1]);
        for(int i = 2; i < n; ++i){
            int left = arr[i] * max_num[i - 1] + dp[i - 1];
            int right = (arr[i] * arr[i - 1])+ dp[i - 2] + max(arr[i] , arr[i - 1]) * max_num[i-2];
            //if(i == n-1) cout<< right<<endl;
            dp[i] = min(left, right);
            max_num[i] = max(arr[i], max_num[i - 1]); 
        }
        return dp[n - 1];
    }
```
