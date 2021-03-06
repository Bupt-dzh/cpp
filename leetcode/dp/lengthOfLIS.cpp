/*
300. 最长上升子序列
给定一个无序的整数数组，找到其中最长上升子序列的长度。

示例:

输入: [10,9,2,5,3,7,101,18]
输出: 4 
解释: 最长的上升子序列是 [2,3,7,101]，它的长度是 4。
说明:

可能会有多种最长上升子序列的组合，你只需要输出对应的长度即可。
你算法的时间复杂度应该为 O(n2) 。
进阶: 你能将算法的时间复杂度降低到 O(n log n) 吗?
*/

/**
 * 思路：
 * 
 * 一、暴力解法
 * 从左至右将每一项作为开头遍历
 * 遇到比前一项大的就记录下来，直至末尾
 * 然后跳至开头，从第二个比开头大的开始记录，直至末尾
 * 重复，直到最后一个比开头大的项
 * 记录所有这些项中最大的项数
 * 然后将开头右移一位，直至末尾
 * 
 * 复杂度O(n3)
 * 没有任何地剪枝，没有利用任何之前计算的结果
 * 
 * 二、动态规划
 * 
 * 满足最优子结构（求解过程包含了所有子结构的解）
 * 无后效性，只关心当前状态而不关心如何得到，前一个状态不受后面的影响
 * 重叠子问题，一个状态可以由多个不同的子序列得到
 * 
 * 先考虑递归关系
 * 能否从n-1项的最长上升子序列，推出n项的
 * { f(n) = f(n-1)
 * { f(n) = f(n-1) + 1
 * 使用哪一个递推式取决于n-1项的最长（上升）子序列的最后一项是否小于第n项
 * 式二还包括了第二种情形：次长子序列最后一项小于第n项的情况
 * 所以我们需要两个东西：
 * 1. 所有最长子序列的长度，及其中最小的最后一项
 * 2. 次长子序列的长度（最长-1)，及其中最小的最后一项
 * 
 * 基础情形: 长度为1的最长上升子序列长度为1
 * 
 * 然后考虑定义状态：
 * 定义状态为所求的解, DP{n} = 前n项最长上升子序列的长度
 * 这样在大多数情况都是可行的
 * 但在这里貌似不太行
 * 
 * 状态，应该是包含我们需要的所有信息的
 * 应该是我们需要知道的关于子结构的所有信息
 * 最终目标解将由这些状态得出
 * 也就是所说的最优子结构（包含所有子结构的解）和无后效性（只关注状态）的体现
 * 
 * 但如果如上面的状态定义
 * 我们无从得知，最后一项的值
 * 
 * 而如果只是简单地把这个值作为状态的一部分
 * 那么它的递推关系（状态转移）又该如何定义呢
 * 
 * 一个状态仅仅应该由子结构的解（状态）得到
 * 如果将这个（最长上升）序列定义为状态
 * 那么所有次长序列中最后一项最小的序列这个信息，又从哪里得到呢
 * 该（最后一项最小的）次长序列并不能由最长（上升）序列，或其他次优得到
 * 至少是很复杂的
 * 
 * 考虑一个最长序列，最后一项最小为5
 * 次优序列，最后一项最小为6
 * 当前第n项值为4
 * 没有办法加到上述二者任何一项后面
 * 因此转移得到的最长序列最后一项最小为5
 * 但次优序列的最后一项最小为6或者4
 * 这取决于是否有比次优序列长度少1的子序列，其最后一项比4小
 * 而这个信息（状态）是没有记录的
 * 
 * 因此这样定义的状态是有问题的
 * 这里只是做一个说明
 * 实际问题中如果发现，状态遗漏了我们包含的信息
 * 那么就需要重新定义状态了
 * 
 * 在这里我们需要"更进一步"
 * 
 * 因为需要知道最后一项的信息，
 * 所以我们不如直接定义状态DP{n}为：
 * 以第n项为最后一项的最长子序列长度
 * （很常用的方法：比如说前缀和）
 * 
 * 这样状态转移方程就变为：
 * DP{n} = max(DP{i} + 1, DP{j}), 其中{int i | nums[i] < nums[n]}, {int j | nums[j] >= nums[n]}
 * 
 * 这样做的时间复杂度是O(n2)
 * 从头至尾一次遍历
 * 每次遍历都需要从所有之前的状态中搜索得到一个最大值
 * 
 * 继续思考压缩状态
 * 上述解法，每一步都需要用到之前每一步的状态信息
 * 需要1 + 2 + 3 + ... + n-1 = n(n-1)/2步
 * 没有办法压缩状态，我们需要在所有已知状态中查找得到最大值
 * 而这是一个无序数组, 复杂度为O(n)
 * 
 * 因为无法压缩状态，也绕不开查找这一步
 * 就需要想办法在查找上面做文章
 * 
 * 回到确定采用哪一种递推式
 * 我们需要的两个东西：
 * 1. 所有最长子序列的长度，及其中最小的最后一项
 * 2. 次长子序列的长度（最长-1)，及其中最小的最后一项
 * 
 * 上面状态定义本身包含了一个确定的信息：最后一项的值
 * 现在我们换一种状态定义：
 * DP{n} = 长度为n的最长(上升)子序列中，最小的最后一项的值
 * 转移方程：
 * DP{n} = min(nums[i] > DP{n-1} ? nums[i] : DP{n}, DP{n})
 *
 *
 * 计算过程：
 * 维护一个状态数组dp
 *
 * 从头至尾遍历整个数组
 * 每当遍历到一个值nums[i]时
 * 从头到尾更新状态数组dp
 *
 * 小朋友你是否有很多问号
 * 这样不也是O(n2)吗
 *
 * 我们需要更新的是：
 * 第一个比nums[i]大的状态DP{j}
 * 即满足：DP{j-1} < nums[i] && DP{j} > nums[i]的第一项
 * 然后再往后依次更新DP{j+1}...
 *
 * 但事实上从递推关系（转移方程）我们也能看出
 * dp是一个单增的数组
 *
 * DP{j+k} > DP{j} > nums[i]
 * 后续的状态都不需要更新, 因为肯定比nums[i]大
 * (如果DP{j}等于nums[i]，那么就没有必要更新)
 *
 * 所以问题就变为了查找第一个比nums[i]大的dp[j]
 * 查找到了就更新为nums[i]
 * 否则就在最后新加一项nums[i]
 *
 * 而在有序的dp数组中查找
 * 复杂度可以优化为O(logN)
 *
 * 因此总的复杂度变为O(nlogN)
 * 
 * 从上面两个状态的定义我们可以知道：
 * 1. 状态的定义会影响状态转移和求解过程
 * 2. 一般来说，定义所求问题的解为状态即可
 *    行不通的时候，需要定义的状态以外的额外条件的时候
 *    修改状态的定义，使之隐含所需的条件(上述可行的两种定义都是如此)
 * 3. 考虑能否压缩状态，如果不行，思考能不能进行查找优化
 */

#include<vector>
#include<iostream>
using std::vector;
using std::size_t;

int lengthOfLIS(vector<int>& nums) {
    // 定义dp[i]为，以nums[i]为最后一项的最长上升子序列长度 
    vector<int> dp{};

    size_t len;
    int ans = 1;
    int max_len;

    // DP{n} = max(DP{i} + 1, DP{j}), 其中{int i | nums[i] < nums[n]}, {int j | nums[j] >= nums[n]}
    for (auto& n : nums)
    {
        max_len = 1;
        len = dp.size();
        for (size_t i = 0; i < len; ++i)
        {
            if (nums[i] < n)
                if (dp[i] + 1 > max_len)
                    max_len = dp[i] + 1;
            else
                if (dp[i] > max_len)
                    max_len = dp[i];
            
        }
        printf("push in: %d\n", max_len);
        dp.push_back(max_len);

        if (max_len > ans)
            ans = max_len;
    }

    return ans;
}

// 二分查找，返回数组中第一个>=目标的位置
int binarySearch(vector<int>& dp, int tar);

int length_of_LIS(vector<int>& nums) {
    // 定义dp[j]为长度为j的所有最长上升子序列当中，最后一项的最小值
    vector<int> dp{};
    int pos;
    for (auto& n : nums)
    {
        // 此处可以优化，先和最后一项比较再查找
        // 不过这里先不改方便理解吧...
        pos = binarySearch(dp, n);
        printf("pos: %d\n", pos);
        if (pos < 0)
            dp.push_back(n);
        else
            dp[pos] = n;

        for (auto& x : dp)
            printf("%d ", x);

        // std::cout << std::endl;
    }
    return dp.size();

}

// 二分查找还需要熟悉，很常用的算法需要做到滚瓜烂熟
int binarySearch(vector<int>& dp, int tar) {
    int i = 0;
    int j = static_cast<int>(dp.size()) - 1;
    int mid;int ans = 0;
    while(i <= j) // 这里可以取等，因为mid会左/右移一位
    {
        mid = (i+j)/2;
        if (dp[mid] < tar)
        {
            i = mid + 1;
            ans = mid + 1;
        }
        else if (dp[mid] > tar)
        {
            j = mid - 1;
            ans = mid;
        }
        else
            return mid;
    }

    if (ans > static_cast<int>(dp.size()) - 1)
        ans = -1;

    return ans;
}

int main() {
    vector<int> arr{10,9,2,5,3,7,101,18};
    printf("%d\n", length_of_LIS(arr));
}
