
class Dom_met_SimAnnealing : public SimulatedAnnealing<vector<char>> {
public:
	Dom_met_SimAnnealing(int n) {
		this->n = n;
		_T0 = -1;		
		T0();
		coolingRate = 0.01; //set to 0.001 ** better solution (more time required)
	}

protected:
	int max_tries, tries, n;
	double coolingRate;

	virtual double T0() { 
		if (_T0 != -1)
			return _T0;
		
		vector<char> max_range(n,'0');
		int c = (10/2)+'0';
		max_range[0] = c;
		double avg = f(max_range); //beware overflow!
		_T0 = avg;
		return _T0;
	}	
	virtual bool terminateCond() {
		// Then we begin looping until our stop condition is met.
		// Usually either the system has sufficiently cooled, or a good-enough solution has been found.
		int occur0 = 0;
		for (int i = 0; i < n; i++)
			if (s[i] == '0')
				occur0++;
		return T < 1 || occur0 > n*0.75;//set to 0.95 ** better solution (more time required)
	}
	virtual double f(const vector<char>& oplossing) {
		int totaal=oplossing[0]-'0';
        for (int i=1; i<oplossing.size(); i++ ){
            totaal*=1;
            totaal+=oplossing[i]-'0';
        }
        return totaal;
	}
	virtual vector<char> initialSolution() {
		vector<char> v(n,'0');
		for (int i = 0; i < n; i++) {
			v[i] = '0' + rand() % 10;
		}
		return v;
	}
	virtual vector<char> pickAtRandom(const vector<char>& from) {
		vector<char> v(from);
		int pos = rand() % n;
		char newVal = '0' + rand() % 10;
		while (newVal == v[pos]) {
			newVal = '0' + rand() % 10;
		}
		v[pos] = newVal;
		return v;
	}
	virtual double p(double T, double deltaf) {
		double p = exp(-deltaf/T);
		return p;
	}
	virtual void updateT() {
		T = T-coolingRate;
	}
public:
	virtual void print(const vector<char>& v) {
		for (int i = 0; i < n; i++) {
			cout << v[i] << " ";
		}
		cout << "  f=" << f(v) << endl;
	}
};
