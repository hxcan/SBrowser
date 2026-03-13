#!/usr/bin/env ruby 

require 'json' #json解析库。
require 'set'

def getNameSet(obj)
    entries=obj['entries'] #获取条目列表。
    
    resultSet=Set.new #结果集合。
    
    entries.each do |currentEntry|
        currentName=currentEntry['name'] #获取名字。
        
        resultSet.add(currentName)
    end
    
    return resultSet
end

#QtCreator中运行的环境变量：
ssnStrJs='/root/SoftwareDevelop/sbrowser/Learn/EnvironmentVariableLearn/environmentVariable.runInQtCreator.json'
json=File.read(ssnStrJs) #读取JSON文件。
obj=JSON.parse(json) #解析JSON。
print("#{obj}\n") #Debug.

#独立运行的环境变量：
ssnStrJsIndependent='/root/SoftwareDevelop/sbrowser/Learn/EnvironmentVariableLearn/environmentVariable.runIndependently.json'
jsonIndependent=File.read(ssnStrJsIndependent) #读取JSON文件。
objIndependent=JSON.parse(jsonIndependent) #解析JSON。
print("#{objIndependent}\n") #Debug.


#找出独立运行情况下未包含的环境变量：
fullNameSet=getNameSet(obj) #计算出完整情况下的环境变量名集合。

independentNameSet=getNameSet(objIndependent) #计算出独立运行情况下的环境变量名集合。

differenceSet=fullNameSet.subtract(independentNameSet) #计算出差集。

print("#{differenceSet.to_a}\n") #Debug.
