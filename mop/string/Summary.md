## >_

第一章《字符串》看完之后的回顾总结

## ..

- 字符串的旋转  
三步旋转，划分、内部旋转（确保内部相对顺序）、外部旋转（交换位置）

- 字符串的包含  
方法一：排序(mlogm + nlogn)后比较(m+n)  
方法二：散列表 + 位运算优化  

- 字符串全排列  
方法一：固定首位递归剩余n-1位  
方法二：按字典序输出  

- 字符串转换成整数  
基本处理是乘以对应位，还需要判断非数字的情况、处理溢出的情况  

- 回文判断  
利用对称性，可以：从两头往中间扫，中间往两头扫  

- 最长回文子串  
方法一：中心扩展算法，枚举中心，对各个中心判断（分奇偶）  
方法二：动态规划算法（略）  
方法三：manacher算法  
