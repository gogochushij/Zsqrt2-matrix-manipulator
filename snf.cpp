///Z[sqrt(2)] matrices manipulator
///gogochushij, 2020

#include <iostream>
#include <vector>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <fstream>

using namespace std;

struct element
{
	int rat, irat;
	struct element operator + (struct element b)
	{
		return {rat+b.rat, irat+b.irat};
	}
	struct element operator -()
	{
		return {-rat, -irat};
	}
	struct element operator -(struct element b)
	{
		return {rat-b.rat, irat-b.irat};
	}
	struct element operator *(struct element b)
	{
		return {rat*b.rat+2*irat*b.irat, irat*b.rat+rat*b.irat};
	}
	struct element operator *(int c)
	{
		return {rat*c, irat*c};
	}
	bool operator ==(struct element b)
	{
	    return (rat==b.rat and irat==b.irat);
	}
	bool operator !=(struct element b)
	{
	    return (rat!=b.rat or irat!=b.irat);
	}
};
struct element make_elem(int rat, int irat)
{
	struct element r = {rat, irat};
	return r;
}
int norm(struct element e)
{
	return abs(e.rat*e.rat - 2*e.irat*e.irat);
}

vector<vector<vector<struct element>>> hist;
vector<vector<struct element>> arr;
int hei, wid;
vector<vector<bool>> ignored;

string out (struct element z, bool align = true)
{
	string sq = "<2>";
	string ir = "";
	if (z.irat != 0)
	{
		if (z.irat == 1)
			ir = sq;
		else if (z.irat == -1)
			ir = "-"+sq;
		else
			ir = to_string(z.irat) + sq;
	}

	string ra = "";
	if (z.rat != 0)
	{
		ra = to_string(z.rat);
		if (ir != "" and z.rat > 0) ra = "+" + ra;
	}

	string res = ir+ra;
	if (res=="") res = "0";
	if (align == true)
    {
        while (res.length() < 8)
            res = " " + res;
    }
	return res;
}

void print()
{
    int mi = -1, mj = -1;
    for (int i = 0; i < hei; i++)
    {
        for (int j = 0; j < wid; j++)
        {

            if (ignored[i][j] == false and arr[i][j] != make_elem(0, 0))
            {
                if (mi==-1)
                    mi = i, mj = j;
                else if (norm(arr[i][j]) < norm(arr[mi][mj]))
                    mi = i, mj = j;
            }
        }
    }


	for (int i = 0; i < hei; i++)
	{
		for (int j = 0; j < wid; j++)
		{
		    if (i == mi and j == mj)
            {
                cout << "(" << out(arr[i][j]) << ") ";
            }
            else if (ignored[i][j] == true)
            {
                cout << "#" << out(arr[i][j]) << "# ";
            }
            else
                cout << out(arr[i][j]) << " ";
		}
		cout << endl;
	}
}

void parse(string command)
{
	stringstream ss;
	ss << command;
	string pred; vector<int> args; int k;
	ss >> pred;
	while (ss >> k)
		args.push_back(k);

	vector<vector<struct element>> parr = arr;
	if (pred == "addrow" or pred == "addcol")
	{
		if (args.size() == 4 or args.size() == 2)
		{
			int to = args[0]-1, from = args[1]-1, rat, irat;
			if (args.size() == 4)
				rat = args[2], irat = args[3];
			else
				rat = 1, irat = 0;

			if (pred == "addrow")
			{
				if (0 <= from and from < hei and 0 <= to and to < hei)
				{
				    hist.push_back(arr);
					for (int i = 0; i < wid; i++)
					{
						arr[to][i] = arr[to][i] + arr[from][i]*make_elem(rat, irat);
					}
				}
				else
				{
					cout << "(X) Range error" << endl;
				}

			}
			else if (pred == "addcol")
			{
				if (0 <= from and from < wid and 0 <= to and to < wid)
				{
				    hist.push_back(arr);
					for (int i = 0; i < hei; i++)
					{
						arr[i][to] = arr[i][to] + arr[i][from]*make_elem(rat, irat);
					}
				}
				else
				{
					cout << "(X) Range error" << endl;
				}
			}

		}
		else
		{
			cout << "(X) Wrong number of arguments, must be 4 or 2" << endl;
		}
	}
	else if (pred == "perrow" or pred == "percol")
	{
		if (args.size() == 2)
		{
			int i = args[0]-1, j = args[1]-1;
			if (pred == "perrow")
			{
				if (0 <= i and i < hei and 0 <= j and j < hei)
				{
				    hist.push_back(arr);
					for (int k = 0; k < wid; k++)
					{
						swap(arr[i][k], arr[j][k]);
					}
				}
				else
				{
					cout << "(X) Range error" << endl;
				}
			}
			else if (pred == "percol")
			{
				if (0 <= i and i < wid and 0 <= j and j < wid)
				{
				    hist.push_back(arr);
					for (int k = 0; k < hei; k++)
					{
						swap(arr[k][i], arr[k][j]);
					}
				}
				else
				{
					cout << "(X) Range error" << endl;
				}
			}
		}
		else
		{
			cout << "(X) Wrong number of arguments: must be 2" << endl;
		}
	}
	else if (pred == "mulrow" or pred == "mulcol")
	{
		if (args.size() == 3)
		{
			int i = args[0]-1; struct element a = {args[1], args[2]};
			if (pred == "mulrow")
			{
				if (0 <= i and i < hei)
				{
				    hist.push_back(arr);
				    cout << "(i) Multiplied row " << i+1 << " by " << out(a,0) << endl;
				    if (norm(a) != 1)
                        cout << "(!) Norm warning: " << out(a,0) << " is non-invertible. Type 'undo' to cancel the operation." << endl;
					for (int k = 0; k < wid; k++)
						arr[i][k] = arr[i][k] * a;
				}
				else
				{
					cout << "(X) Range error" << endl;
				}
			}
			else if (pred == "mulcol")
			{
				if (0 <= i and i < wid)
				{
				    hist.push_back(arr);
				    cout << "(i) Multiplied column " << i+1 << " by " << out(a,0) << endl;
				    if (norm(a) != 1)
                        cout << "(!) Norm warning: " << out(a,0) << " is non-invertible. Type 'undo' to cancel the operation." << endl;
					for (int k = 0; k < hei; k++)
						arr[k][i] = arr[k][i] * a;
				}
			}
		}
		else
        {
            cout << "(X) Wrong number of arguments: must be 3" << endl;
        }
	}
	else if (pred == "ignore")
    {
        if (args.size() == 2)
        {
            int i = args[0]-1, j = args[1]-1;
            if (0 <= i and i < hei and 0 <= j and j < wid)
            {
                ignored[i][j] = !ignored[i][j];
            }
            else
            {
                cout << "(X) Range error" << endl;
            }
        }
        else
        {
            cout << "(X) Wrong number of arguments: must be 2" << endl;
        }
    }
	else if (pred == "undo")
    {
        if (hist.size() > 0)
        {
            arr = hist.back();
            hist.erase(hist.end()-1);
        }
        else
        {
            cout << "(X) Can't undo" << endl;
        }

    }
	else
	{
		cout << "(X) No such command: " << pred << endl;
	}

    if (hist.size() > 10)
    {
        hist.erase(hist.begin());
    }

}

bool read_matrix()
{
    ifstream in ("matrix.txt");
    string tt;
    int j=1;
    while (getline(in, tt))
    {
        stringstream ss;
        ss << tt;
        int m; vector<int> nums;
        while (ss >> m)
            nums.push_back(m);

        if (nums.size() % 2 == 0)
        {
            if ((int)arr.size() > 0)
            {
                if (nums.size()/2 != arr.back().size())
                {
                    cout << "<XXX> Error loading matrix: different amount of arguments at lines " << j-1 << " and " << j << endl;
                    return false;
                }

            }
            arr.push_back(vector<struct element>(nums.size()/2));
            for (int i = 0; 2*i < nums.size(); i++)
            {
                arr.back()[i] = make_elem(nums[2*i], nums[2*i+1]);
            }
        }
        else
        {
            cout << "<XXX> Error loading matrix: found unpaired number at line " << j << endl;
            return false;
        }
        j++;
    }
    hei = arr.size(), wid = arr[0].size();
    ignored = vector<vector<bool>> (hei, vector<bool>(wid, false));
    if (arr.size() > 0)
        return true;
    else
    {
        cout << "<XXX> Error loading matrix: can't read from file matrix.txt or no matrix found" << endl;
        return false;
    }
}

int main()
{
    cout << "(i) *** Z[<2>] Matrix manipulator, 25 may 2020 ***" << endl;
    cout << "(i) Reading matrix from matrix.txt..." << endl;
    if (read_matrix() == false)
    {
        cout << "(i) Execution stopped due to reading error";
        return 0;
    }
    cout << "(i) Success" << endl;
	while (true)
	{
		print();
		string comm;
		cout << "Command: "; fflush(stdout);
		getline(cin, comm);

		parse(comm);
	}
}
