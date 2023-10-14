/*+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
简述：
	文件夹遍历（非递归）
操作：
	运行exe，显示exe所在的文件夹下的所有文件的路径
解释：
	GetCurrentDirectoryA()  获取当前文件夹的路径

	FindFirstFileA()  查找目录中第一个与特定名称匹配的文件或文件夹
		// 使用通配符*则为部分名称
		// 函数将打开一个搜索句柄，关闭搜索句柄使用 FindClose()，而不是 CloseHandle()

	FindNextFileA()  继续从上一次调用 FindFirstFile() 之后的文件搜索
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+*/

#include<windows.h>
#include<iostream>
using namespace std;

// 手动实现  链式栈结构
class Node
{
public:
	Node* nextNode;
	HANDLE fileSearchHandle;
	char folderPath[MAX_PATH];

	Node(Node* _nextNode, char* _folderPath)
	{
		nextNode = _nextNode;
		fileSearchHandle = INVALID_HANDLE_VALUE;
		memset(folderPath, 0, MAX_PATH);
		strcpy(folderPath, _folderPath);

		return;
	}
	~Node(void)
	{
		if (INVALID_HANDLE_VALUE != fileSearchHandle)
		{
			FindClose(fileSearchHandle);
		}
		return;
	}
};
class Stack
{
public:
	Node* topNode;

	Stack(void)
	{
		topNode = NULL;
		return;
	}
	~Stack(void)
	{
		for (; Pop(););
		return;
	}

	bool isEmpty(void)
	{
		return (NULL == topNode) ? true : false;
	}
	void Push(char* folderPath)
	{
		topNode = new Node(topNode, folderPath);
		return;
	}
	int Pop(void)
	{
		if (isEmpty())
		{
			return 0;
		}
		Node* p = topNode;
		topNode = topNode->nextNode;
		delete p;
		return 1;
	}

	char* GetTopFolderPath(void)
	{
		return topNode->folderPath;
	}
	HANDLE GetTopHandle(void)
	{
		return topNode->fileSearchHandle;
	}
	void ReviseTopHandle(HANDLE newHandle)
	{
		topNode->fileSearchHandle = newHandle;
		return;
	}
};

void FolderWalk(char* folderPath)
{
	char tempPath[MAX_PATH] = { 0 };
	WIN32_FIND_DATAA fileData;
	HANDLE FileSearchHandle = NULL;
	Stack stack;
	stack.Push(folderPath);


	for (;;)
	{
		// 查找栈顶文件夹内的第一个文件（不区分文件夹和文件）
		sprintf(tempPath, "%s\\*.*", stack.GetTopFolderPath());
		FileSearchHandle = FindFirstFileA(tempPath, &fileData);

		if (INVALID_HANDLE_VALUE != FileSearchHandle)
		{
			// 有找到第一个文件，修改栈顶结点中的句柄成员变量
			stack.ReviseTopHandle(FileSearchHandle);
		}
		else
		{
			// 查找失败（可能是本函数运行途中，文件夹被删除或被重命名，也可能是其他原因）
			for (;;)
			{
				stack.Pop();// 弹栈
				if (stack.isEmpty())
				{
					return;// 若栈为空，结束遍历
				}
				// 栈顶降了一层，找下一个文件，若没有下一个文件则继续弹栈
				if (FindNextFileA(stack.GetTopHandle(), &fileData))
				{
					break;// 有找到下一个
				}
			}
		}


		for (;;)
		{
			// 细分找到的是文件还是文件夹
			if (FILE_ATTRIBUTE_DIRECTORY == fileData.dwFileAttributes)
			{
				// 是文件夹

				// 将文件夹路径压栈
				/*if ('.' != fileData.cFileName[0])
				{
					memset(tempPath, 0, MAX_PATH);
					sprintf(tempPath, "%s\\%s", stack.GetTopFolderPath(), fileData.cFileName);
					stack.Push(tempPath);

					break;
				}*/
			}
			else
			{
				// 是文件

				cout << stack.GetTopFolderPath() << "\\" << fileData.cFileName << endl;// 打印文件路径
			}


			// 找下一个文件
			for (;;)
			{
				if (FindNextFileA(stack.GetTopHandle(), &fileData))
				{
					break;// 有找到
				}
				// 未找到下一个文件，即已是末尾
				stack.Pop();// 弹栈
				if (stack.isEmpty())
				{
					return;// 若栈为空，结束遍历
				}
			}
		}
	}
}

int main(void)
{
	char folderPath[MAX_PATH] = { 0 };
	GetCurrentDirectoryA(MAX_PATH, folderPath);

	FolderWalk(folderPath);

	system("pause");
	return 0;
}
