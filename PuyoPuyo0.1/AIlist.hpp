#pragma once

//AIのリストの管理とCSVへの記入を行うクラス
class AIlist {
private:
	Array<String> AIname;
	Array<FilePath> filepath;
	CSV csv;
public:
	AIlist();
	size_t size()const;
	void addAI(const String& name, String& path);
	const Array<String>& AInameArray();
};
