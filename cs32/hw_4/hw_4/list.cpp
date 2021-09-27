void listAll(const MenuItem* m, string path) // two-parameter overload
{
	if (m != NULL)
	{
		if (m->name() != "")
		{
			cout << path + m->name() << endl;
			path += m->name() + "/";
		}
		if (m->menuItems() != NULL)
			for (MenuItem* mi : *(m->menuItems()))
				listAll(mi, path);
	}
}