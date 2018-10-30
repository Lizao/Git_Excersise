# git遇到的问题以及解决方案
## 1.多次使用git reset 以至于版本回退过多
### 解决方案：
使用
> git reflog    #找到远程仓库的所有版本，获取hash Id
>
> git reset <hash Id>  #版本可切换到任意版本

## 2.提示"Your branch is ahead of 'origin/master' by 2 commits"导致无法push至远程仓库
### 解决方案：
使用:需要注意创建新的分支
>git log --oneline -5 # 获取远程提交记录 hash Id
>
>git reset --hard <hash Id> # 切换到指定版本
>
>git pull   #拉代码
>
>git fetch  #使用igerrit上面的cherrypick, 解决冲突,将conflict项add至暂存区
>
>git cherry-pick --continue #至此问题解决

 **git pull <远程主机名> <远程分支名> **



## 3.在master分支修改A和B，现在需要对A添加新功能，使用创建分支以保证代码安全，步骤如下：

> git checkout -b newA 创建新的分支newA
>
> git add /git commit 修改内容
>
> git checkout master 切换到主分支
>
>git merge newA 将分支内容与主分支进行合并
>
>subl CONFLICT File 一一解决冲突文件
>
>git add CONFLICT File 将冲突文件添加到暂存区
>
>git commit -m 后使用 git commit --amend 直接用git commit -- amend 会报错
>
>git push 至此问题解决完毕



**上传代码前需要pull一下，merge过后再上传**




https://www.cnblogs.com/dsxniubility/p/5817776.html
