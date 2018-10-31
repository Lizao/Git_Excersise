# <font face="黑体"> git遇到的问题以及解决方案 </font>
## 1.多次使用git reset 以至于版本回退过多
### 解决方案：
```
git reflog           #找到远程仓库的所有版本，获取hash Id
git reset <hash Id>  #版本可切换到任意版本
```

## 2.提示"Your branch is ahead of 'origin/master' by 2 commits"导致无法push至远程仓库
### 解决方案：
需要注意创建新的分支
```
<<<<<<< HEAD
git log --oneline -5       # 获取远程提交记录 hash Id
git reset --hard <hash Id> # 切换到指定版本
git pull                   #拉代码
git fetch                  #使用igerrit上面的cherrypick, 解决冲突,将conflict项add至暂存区
git cherry-pick --continue #至此问题解决
```
### 注： **git pull <远程主机名> <远程分支名> **
 
## 3.在master分支修改A和B，现在需要对A添加新功能，使用创建分支以保证代码安全，步骤如下：

```
git checkout -b newA 创建新的分支newA
git add /git commit 修改内容
git checkout master 切换到主分支
git merge newA 将分支内容与主分支进行合并
subl CONFLICT File 一一解决冲突文件
git add CONFLICT File 将冲突文件添加到暂存区
git commit -m 后使用 git commit --amend 直接用git commit -- amend 会报错
git push 至此问题解决完毕
```
 ### 注： **git pull <远程主机名> <远程分支名> **

**上传代码前需要pull一下，merge过后再上传**

### 4. 在gerrit上面提交的代码已被review，但是出现Can not merge。新建分支，cherry-pick后push出现
```
error: could not apply 58c9955f... implement function of setting volume
hint: after resolving the conflicts, mark the corrected paths
hint: and commit the result with 'git commit'
Resolved 'voicerecoglib/sds/Local/Build/VR_SDSBuildAction.cpp' using previous resolution.
Resolved 'voicerecoglib/sds/Local/VR_SDSLocal_DEF.h' using previous resolution.
```

### 解决方案
```
1.git log --oneline -number #回退版本号 number
2.git reset --hard  number  #回退至版本
3.git pull                  #拉取该版本代码
4.cherry-pick gerrit        #待提交的版本(此时会出现问题中的error)
5.git status / git add
6.git commit -c 58c9955f    #(关键步骤)
7.git push
注：出现Can not Merge 原因是多人同时开发，某A代码提交后没有被review，即没有及时merge。B提交后，代码先被merge，但与A代码冲突，导致A的代码不能被merge。
```

### git pull 与 git fetch 区别
```
git  pull   从远程拉取最新版本到本地  自动合并merge            
git pull origin master
git  fetch   从远程获取最新版本 到本地  不会自动合并 merge    
git fetch  origin master git log  -p master ../origin/master  git merge orgin/master
实际使用中  使用git fetch 更安全    在merge之前可以看清楚 更新情况  再决定是否合并
```

## [参考链接](https://www.cnblogs.com/dsxniubility/p/5817776.html)
