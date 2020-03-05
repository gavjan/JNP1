#ifndef PLAYLIST_LIB_PLAYLIST_H
#define PLAYLIST_LIB_PLAYLIST_H
#include <list>
#include <cassert>
#include <utility>
#include <vector>
#include <memory>
#include <algorithm>
#include <random>
#include <iostream>

#define SHUFFLE 0
#define SEQUENCE 1
#define ODDEVEN 2
#define SONG 0
#define MOVIE 1
#define UNSUPPORTED 2
#define CURROPTFILE 3
#define CURROPTCONTENT 4
struct mode {
	int type;
	std::default_random_engine engine;
};
struct mode createShuffleMode(unsigned seed) {
	struct mode m{};
	m.engine.seed(seed);
	m.type=SHUFFLE;
	return m;
}
struct mode createOddEvenMode() {
	struct mode m{};
	m.type=ODDEVEN;
	return m;
}
struct mode createSequenceMode() {
	struct mode m{};
	m.type=SEQUENCE;
	return m;
}
bool isSpecial(char c) {
	return c==',' || c=='.' || c=='!' || c=='?' || c=='\'' || c==':' || c==';' || c=='-';
}
bool startsWith(const std::string& str,const std::string& head) {return str.rfind(head, 0) == 0;}
std::vector<std::string> splitWithLine(const std::string& line) {
	std::vector<std::string> tokens;
	unsigned long i = 0, last = 0;
	while ((i = line.find('|', last)) != std::string::npos) {
		tokens.push_back(line.substr(last, i-last));
		last = i+1;
	}

	tokens.push_back(line.substr(last, std::string::npos));

	return tokens;
}
class File {
public:
	bool has_artist=false;
	bool has_year=false;
	bool has_title=false;
	std::string title;
	std::string artist;
	std::string year;
	std::string content;
	std::vector<std::pair<std::string,std::string>> metadata;
	int type;
	explicit File(std::string str) {
		type=UNSUPPORTED;
		if (startsWith(str,"audio|")) {
			type=SONG;
			str.erase(str.begin(), str.begin()+6);
		}
		else if (startsWith(str,"video|")) {
			type=MOVIE;
			str.erase(str.begin(), str.begin()+6);
		}
		else {
			type=UNSUPPORTED;
			if(str.find('|')==-1) type=CURROPTFILE;
		}
		if(type!=UNSUPPORTED) {
			auto tokens=splitWithLine(str);
			int i=0,n=tokens.size();
			for(const auto& x: tokens) {
				if(i==(n-1)) break;
				if(x.find(':')==-1) type=CURROPTFILE;
				std::string meta = x.substr(0,x.find(':'));
				std::string data = x.substr(x.find(':')+1);

				bool has_already=false;
				for(auto j : metadata)
					if(j.first==meta) { j.second=data; has_already=true;}
				if(!has_already) {
					std::pair<std::string,std::string> meta_data(meta,data);
					metadata.push_back(meta_data);
				}
				if(startsWith(x,"title:")) {has_title=true;title=x.substr(x.find(':')+1);}
				if(startsWith(x,"artist:")) {has_artist=true;artist=x.substr(x.find(':')+1);}
				if(startsWith(x,"year:")) {has_year=true;year=x.substr(x.find(':')+1);}

				i++;
			}
			content=tokens.back();
			for(char& c : content)
				if(!(isspace(c) || isalnum(c) || isSpecial(c))) type=CURROPTCONTENT;
		}
	}

	explicit File(const char* str) : File(std::string(str)) {
		assert(str != nullptr);
	}

	bool isUnsupported() {
		return type==UNSUPPORTED;
	}
	bool isCorruptFile() {
		return type==CURROPTFILE;
	}
	bool isCorruptContent() {
		return type==CURROPTCONTENT;
	}
};
class Playlist {
public:
	std::string title;
	std::string artist;
	std::string year;
	explicit Playlist(std::string str) : title(std::move(str)){
		mode.type=SEQUENCE;
	}
	virtual void play() {
		std::cout<<"Playlist [";
		std::cout<<title;
		std::cout<<"]"<<std::endl;
		if(mode.type==SEQUENCE) {
			for(const auto& x: list)
				x->play();
		}
		if(mode.type==ODDEVEN) {
			std::vector<std::shared_ptr<Playlist>> temp(list.size());
			std::copy(list.begin(), list.end(), temp.begin());
			for(int i=1;i<temp.size();i+=2)
				temp[i]->play();
			for(int i=0;i<temp.size();i+=2)
				temp[i]->play();
		}
		if(mode.type==SHUFFLE) {
			std::vector<std::shared_ptr<Playlist>> temp(list.size());
			std::copy(list.begin(), list.end(), temp.begin());

			std::shuffle(temp.begin(), temp.end(),mode.engine);

			for(const auto& x: temp)
				x->play();
		}

	}
	virtual void add(std::shared_ptr<Playlist>& element) {
		list.push_back(element);
	}
	virtual void add(std::shared_ptr<Playlist>& element, unsigned pos) {
		auto it=list.begin();
		for(int i=0;i<pos;i++) it++;
		list.insert(it, element);
	}
	virtual void remove(){
		list.pop_back();
	}
	virtual void remove(unsigned pos){
		auto it=list.begin();
		for(int i=0;i<pos;i++) it++;
		list.erase(it);
	}
	virtual void setMode(struct mode m) {
		mode=m;
	}
protected:
	struct mode mode{};
	std::list<std::shared_ptr<Playlist>> list;
};
class PlayerException {
public:
	explicit PlayerException(std::string str){ ex=std::move(str);}
	explicit PlayerException(const char* str): PlayerException(std::string(str)) {}
	[[nodiscard]] std::string what() const {return ex;}
private:
	std::string ex;
};
class Song : public Playlist {
public:
	std::vector<std::pair<std::string,std::string>> metadata;
	std::string content;
	explicit Song(const File& f) : Playlist(f.title) {
		metadata=f.metadata;
		artist=f.artist;
		year=f.year;
		content=f.content;
	}
	void play() override {
		std::cout<<"Song [";
		std::cout<<artist;
		std::cout<<", ";
		std::cout<<title;
		std::cout<<"]: ";
		std::cout<<content<<std::endl;
	}
};
class Movie : public Playlist {
public:
	std::vector<std::pair<std::string,std::string>> metadata;
	std::string content;
	explicit Movie(File f) : Playlist(f.title) {
		title=f.title;
		artist=f.artist;
		year=f.year;
		metadata=f.metadata;
		content=f.content;
	}
	void play() override {
		std::cout<<"Movie [";
		std::cout<<title;
		std::cout<<", ";
		std::cout<<year;
		std::cout<<"]: ";
		std::cout<<"Displaying ";
		std::cout<<title<<std::endl;
	}
};
class Player {
public:
	std::shared_ptr<Playlist> openFile(File f) {
		if(f.isUnsupported()) {throw PlayerException("unsupported type");}
		if(f.isCorruptFile()) {throw PlayerException("corrupt file");}
		if(f.isCorruptContent()) {throw PlayerException("corrupt content");}

		std::shared_ptr<Playlist> ret;
		if(f.type==SONG && f.has_title && f.has_artist) {
			auto song = std::make_shared<Song>(f);
			ret = std::static_pointer_cast<Playlist>(song);
		}
		else if(f.type==MOVIE && f.has_title && f.has_year) {
			auto movie=std::make_shared<Movie>(f);
			ret=std::static_pointer_cast<Playlist>(movie);
		}
		else {throw PlayerException("unsupported type");}
		return ret;
	}
	std::shared_ptr<Playlist> createPlaylist(const std::string& str) {
		return std::make_shared<Playlist>(str);
	}
	std::shared_ptr<Playlist> createPlaylist(const char* str) {
		assert(str != nullptr);
		return createPlaylist(std::string(str));
	}
};

#endif //PLAYLIST_LIB_PLAYLIST_H
