#include <iostream>
#include <string>
using namespace std;

/*
HOMEWORK 3
Christian Yu
104785497
CS32-2 w/ Smallberg
*/

class File
{
public:
	File(string name) : m_name(name) {}						//simple constructor, initialize name only
	string name() const { return m_name; }					//returns name for access

	virtual void open() const = 0;							//pure virtual, differs based on file type
	virtual void redisplay() const { cout << "refresh the screen"; }	//same used by 2 of 3 classes given

	virtual ~File() { cout << endl; }						//print new line, since is default for all destructor messages
private:
	string m_name;
};

class TextMsg : public File
{
public:
	TextMsg(string name) :File(name) { }						//initializes with name
	virtual void open() const
	{
		cout << "open text message";						//must define open function
	}
	~TextMsg()
	{
		cout << "Destroying " << name() << ", a text message";
	}
};

class Video : public File
{
public:
	Video(string name, int length) :File(name), m_length(length) {}	//initializes with name and additional parameter, length
	virtual void open() const
	{
		cout << "play " << m_length << " second video";		//must define open function
	}
	virtual void redisplay() const
	{
		cout << "replay video";								//redefines redisplay
	}
	~Video()
	{
		cout << "Destroying " << name() << ", a video";
	}
private:
	int m_length;
};

class Picture : public File
{
public:
	Picture(string name) :File(name) {}						//initializes with name
	virtual void open() const								//must define open function
	{
		cout << "show picture";
	}
	~Picture()
	{
		cout << "Destroying the picture " << name();
	}
};


void openAndRedisplay(const File* f)
{
	cout << f->name() << ": ";
	f->open();
	cout << endl << "Redisplay: ";
	f->redisplay();
	cout << endl;
}

int main()
{
	File* files[4];
	files[0] = new TextMsg("fromFred.txt");
	// Videos have a name and running time
	files[1] = new Video("goblin.mpg", 3780);
	files[2] = new Picture("kitten.jpg");
	files[3] = new Picture("baby.jpg");

	for (int k = 0; k < 4; k++)
		openAndRedisplay(files[k]);

	// Clean up the files before exiting
	cout << "Cleaning up." << endl;
	for (int k = 0; k < 4; k++)
		delete files[k];
}