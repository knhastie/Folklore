#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <fstream>
#include <iostream>
using namespace std;

//precise random number generation function (between 1 and 0)
double prand()
{
	double val1 = (rand() % 1000);
	double val2 = val1 / 1000;
	return val2;
}

class range
{
	//access
public:
	//members
	bool Increased;
	double Distance;
	//constructor
	range()
	{
		double val = prand();
		//compare to probability that individual is a person of ranged influence (in a position of cultural authority).  This probability can be reset for different simulations
		if (val < 0.05)
		{
			Increased = true;
			if (val < 0.01)
				Distance = 20;
			else
				Distance = 10;
		}
		else
		{
			Increased = false;
			Distance = 5;
		}
	}
};

class Teller
{
	//Access specifier
public:
	//Data members
	double Language;
	double Class;
	double Influence;
	double Influencable;
	range Range;
	double Variability;
	double Rarity;
	double Prestige;
	//Default Constructors
	Teller()
	{
		Language = rand() % 10;
		Class = rand() % 10;
		Influence = prand();
		Influencable = prand();
		Range = range();
		Variability = prand() * 2 - 1;
		Rarity = prand() * 2 - 1;
		double val = prand();
		if (val < 0.7)
			Prestige = prand() / 5;
		else
			Prestige = prand();
	}
	//should add a constructor specifying language so I can make areas that are closer geographically also have similar languages
};

class submotif
{
public:
	bool Present;
	int ID;
	double Success;
	double Developed;
	submotif()
	{
		Present = false;
		ID = rand() % 10;
		Success = prand();
		Developed = 1;
	}
};

class motif
{
public:
	//members
	bool Present;
	int ID;
	double Variation;
	double Success;
	int Capacity;
	submotif* subp;
	//default Constructor
	motif()
	{
		Present = false;
		ID = rand() % 100;
		//should change range for IDs to match actual folktale index
		Variation = prand();
		Success = prand();
		Capacity = 5 + (rand() % 6 - 3);
		//can change Capacity depending on how complex I want stories to get--research folk tales?
		//rand term adds variability to Capacity size, can change that term too (or remove)
		subp = new submotif[Capacity];
	}
};

class Story
{
public:
	//members
	double Genre;
	motif* motifp;
	int Capacity;
	bool Empty;
	//constructor
	Story()
	{
		Empty = true;
		Capacity = 8 + (rand() % 10 - 5);
		Genre = 0;
		motifp = new motif[Capacity];
	}
};

class Collection
{
public:
	//members
	bool Empty;
	Story* storyp;
	int Capacity;
	//constructor
	Collection()
	{
		Empty = true;
		Capacity = 12 + (rand() % 10 - 5);
		storyp = new Story[Capacity];
	}
};

void transmission(Story* pointS, Teller* pointT, int xT1, int xT2)
{
	//have to test this is actually how to pass 2d pointers first
	return;
}

int main()
{
	srand(time(NULL));
	//one story per person, variable motifs and submotifs
	double Pbase = 0.2;
	double Parise = 0.15;
	int mottypes = 100;
	Story matS[100][100];
	Teller matT[100][100];
	//first row
	for (int x = 0; x < 100; x++)
	{
		//determine if motif emerges
		double val = prand();
		if (val < Parise)
		{
			matS[x][0].motifp[0].Present = true;
		}
		//first time step, so there isn't anything to pass on yet
	}
	//all following rows
	for (int t = 0; t < 99; t++)
	{
		//start with vertical evolution
		for (int x = 0; x < 100; x++)
		{
			//chance that motifs pass on
			int cap = matS[x][t].Capacity;
			for (int s = 0; s < cap; s++)
			{
				if (matS[x][t].motifp[s].Present == true)
				{
					double val = prand();
					if (val < 0.4 * (1 + matS[x][t].motifp[s].Success))
					{
						matS[x][t + 1].motifp[s] = matS[x][t].motifp[s];
						//chance that submotif develops
						double val2 = prand();
						if (val2 < Parise / 2)
						{
							if (matS[x][t].motifp[s].subp[1].Present == true)
								matS[x][t + 1].motifp[s].subp[1].Developed = matS[s][t].motifp[s].subp[1].Developed + 1;
							else
								matS[x][t + 1].motifp[s].subp[1].Present = true;
						}
						else
							matS[x][t + 1].motifp[s].subp[1].Present = false;
					}
				}
			}
			//chance that new motif develops
			int* arrEmptySlots;
			arrEmptySlots = new int[cap];
			int numEmptySlots = 0;
			for (int s = 0; s < cap; s++)
			{
				if (matS[0][t].motifp[s].Present == false)
				{
					arrEmptySlots[numEmptySlots] = s;
					numEmptySlots++;
				}
			}
			if (numEmptySlots > 0)
			{
				double val = prand();
				if (val < Parise)
				{
					matS[0][t].motifp[arrEmptySlots[0]].Present = true;
				}
			}
		}

		//determine probability of transmission of story from five future inviduals (x=0)
		for (int xT = 1; xT < 6; xT++)
		{
			//determine if teller has any motifs to transmit
			bool Has = false;
			for (int s = 0; s < matS[xT][t].Capacity; s++)
			{
				if (matS[xT][t].motifp[s].Present == true)
					Has = true;
			}
			//if so, continue
			if (Has)
			{
				//set to base probability of transmission
				double P = Pbase;
				//distance-based probability change
				double Pchange = P * (xT / 10);
				P = Pbase - Pchange;
				//new Pchange with all other factors
				//linguistic and class differences next
				Pchange = 0 - P * (abs(matT[xT][t].Class - matT[0][t].Class)/10 + abs(matT[xT][t].Language - matT[0][t].Language)/10);
				//add teller influence and listener impressionabilty
				Pchange = Pchange + (Pbase * matT[xT][t].Influence) + (Pbase * matT[0][t].Influencable);
				//only factor left is success values of each motif
				double totSuccess = 0;
				int totPresent = 0;
				for (int s = 0; s < matS[xT][t].Capacity; s++)
				{
					if (matS[xT][t].motifp[s].Present)
					{
						totSuccess = totSuccess + matS[xT][t].motifp[s].Success;
						totPresent++;
					}
				}
				double avSuccess = totSuccess / totPresent;
				Pchange = Pchange + (Pbase * avSuccess);
				P = Pchange + Pbase;
				double val = prand();
				if (val < P)
					matS[0][t + 1] = matS[xT][t];
			}
		}
		for (int xL = 1; xL < 100; xL++)
		{
			if (xL < 5)
			{
				//just compare individuals from x=0 to x=xL+5
				for (int xT = 0; xT < xL + 5; xT++)
					//same entire process as for x=0, but using xL instead of zero.
				{
					//determine if teller has any motifs to transmit
					bool Has = false;
					for (int s = 0; s < matS[xT][t].Capacity; s++)
					{
						if (matS[xT][t].motifp[s].Present == true)
							Has = true;
					}

					//if so, continue
					if (Has)
					{
						//set to base probability of transmission
						double P = Pbase;
						//distance-based probability change
						double Pchange = P * (xT / 10);
						P = Pbase - Pchange;
						//new Pchange with all other factors
						//linguistic and class differences next
						Pchange = 0 - P * (abs(matT[xT][t].Class - matT[xL][t].Class)/10 + abs(matT[xT][t].Language - matT[xL][t].Language)/10);
						//add teller influence and listener impressionabilty
						Pchange = Pchange + (Pbase * matT[xT][t].Influence) + (Pbase * matT[xL][t].Influencable);
						//only factor left is success values of each motif
						double totSuccess = 0;
						int totPresent = 0;
						for (int s = 0; s < matS[xT][t].Capacity; s++)
						{
							if (matS[xT][t].motifp[s].Present)
							{
								totSuccess = totSuccess + matS[xT][t].motifp[s].Success;
								totPresent++;
							}
						}
						double avSuccess = totSuccess / totPresent;
						Pchange = Pchange + (Pbase * avSuccess);
						P = Pchange + Pbase;
						double val = prand();
						if (val < P)
							matS[xL][t + 1] = matS[xT][t];
					}
				}
			}
			else if (xL < 95)
			{
				for (int xT = (xL - 5); xT < (xL + 5); xT++)
					//same process
				{
					//determine if teller has any motifs to transmit
					bool Has = false;
					for (int s = 0; s < matS[xT][t].Capacity; s++)
					{
						if (matS[xT][t].motifp[s].Present == true)
							Has = true;
					}

					//if so, continue
					if (Has)
					{
						//set to base probability of transmission
						double P = Pbase;
						//distance-based probability change
						double Pchange = P * (abs(xT-xL) / 5);
						P = Pbase - Pchange;
						//new Pchange with all other factors
						//linguistic and class differences next
						Pchange = 0 - Pbase * (abs(matT[xT][t].Class - matT[xL][t].Class)/10 + abs(matT[xT][t].Language - matT[xL][t].Language)/10);
						//add teller influence and listener impressionabilty
						Pchange = Pchange + (Pbase * matT[xT][t].Influence) + (Pbase * matT[xL][t].Influencable);
						//only factor left is success values of each motif
						double totSuccess = 0;
						int totPresent = 0;
						for (int s = 0; s < matS[xT][t].Capacity; s++)
						{
							if (matS[xT][t].motifp[s].Present)
							{
								totSuccess = totSuccess + matS[xT][t].motifp[s].Success;
								totPresent++;
							}
						}
						double avSuccess = totSuccess / totPresent;
						Pchange = Pchange + (Pbase * avSuccess);
						P = Pchange + Pbase;
						double val = prand();
						if (val < P)
							matS[xL][t + 1] = matS[xT][t];
					}
				}
			}
			else
			{
				for (int xT = xL - 5; xT < 100; xT++)
					//same process
				{
					//determine if teller has any motifs to transmit
					bool Has = false;
					for (int s = 0; s < matS[xT][t].Capacity; s++)
					{
						if (matS[xT][t].motifp[s].Present == true)
							Has = true;
					}

					//if so, continue
					if (Has)
					{
						//set to base probability of transmission
						double P = Pbase;
						//distance-based probability change
						double Pchange = P * (abs(xT-xL)/5);
						P = Pbase - Pchange;
						//new Pchange with all other factors
						//linguistic and class differences next
						Pchange = 0 - Pbase * (abs(matT[xT][t].Class - matT[xL][t].Class)/10 + abs(matT[xT][t].Language - matT[xL][t].Language)/10);
						//add teller influence and listener impressionabilty
						Pchange = Pchange + (Pbase * matT[xT][t].Influence) + (Pbase * matT[xL][t].Influencable);
						//only factor left is success values of each motif
						double totSuccess = 0;
						int totPresent = 0;
						for (int s = 0; s < matS[xT][t].Capacity; s++)
						{
							if (matS[xT][t].motifp[s].Present)
							{
								totSuccess = totSuccess + matS[xT][t].motifp[s].Success;
								totPresent++;
							}
						}
						double avSuccess = totSuccess / totPresent;
						Pchange = Pchange + (Pbase * avSuccess);
						P = Pchange + Pbase;
						double val = prand();
						if (val < P)
							matS[xL][t + 1] = matS[xT][t];
					}
				}
			}
		}
	}
	//making a motif frequency graph
	int frequency[100][100];
	for (int t = 0; t < 100; t++)
	{
		for (int xf = 0; xf < 100; xf++)
		{
			int sum = 0;
			for (int xM = 0; xM < 100; xM++)
			{
				int cap = matS[xM][t].Capacity;
				for (int c = 0; c < cap; c++)
				{
					if ((matS[xM][t].motifp[c].Present) && (matS[xM][t].motifp[c].ID == xf))
						sum++;
				}
			}
			frequency[xf][t] = sum;
			cout << sum << endl;
		}
	}
	//write each frequency column to file
	ofstream freqfile1;
	freqfile1.open("FreqData1.dat");
	for (int i = 0; i < 100; i++)
	{
		for (int b = 0; b < 100; b++)
		{
			freqfile1 << frequency[b][i] << " ";
		}
		freqfile1 << endl;
	}
	freqfile1.close();
	int u;
	cin >> u;
	return 0;
}
