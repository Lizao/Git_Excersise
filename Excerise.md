# git遇到的问题以及解决方案
## 1.多次使用git reset 以至于版本回退过多
### 解决方案：
使用
> git reflog    #找到远程仓库的所有版本，获取hash Id
>
> git reset <hash Id>  #版本可切换到任意版本

## 2.提示"Your branch is ahead of 'origin/master' by 2 commits"导致无法push至远程仓库
### 解决方案：
使用
>git log --oneline -5 # 获取远程提交记录 hash Id
>
>git reset --hard <hash Id> # 切换到指定版本
>
>git pull   #拉代码
>
>git fetch  #使用igerrit上面的cherrypick, 解决冲突,将conflict项add至暂存区
>
>git cherrypick --continue #至此问题解决
