#ifndef TRANGE_H
#define TRANGE_H

//! ”ÍˆÍƒNƒ‰ƒX
template<typename _Tp> class TRange
{
public:
	TRange() : first(0), last(-1)
	{
	};
	TRange(_Tp _first, _Tp _last) : first(_first), last(_last)
	{
	};

	TRange intersected(const TRange& r) const
	{
		TRange intersect;
		if (size() >= r.size()) {
			if ((first <= r.first && last >= r.first)
			 || (first <= r.last && last >= r.last)) {
				if (first >= r.first) {
					intersect.first = first;
				} else {
					intersect.first = r.first;
				}

				if (last <= r.last) {
					intersect.last = last;
				} else {
					intersect.last = r.last;
				}
			}
		} else {
			if ((r.first <= first && r.last >= first)
			 || (r.first <= last && r.last >= last)) {
				if (first <= r.first) {
					intersect.first = r.first;
				} else {
					intersect.first = first;
				}

				if (last >= r.last) {
					intersect.last = r.last;
				} else {
					intersect.last = last;
				}
			}
		}

		return intersect;
	}

	bool isNull() const
	{
		return (last < first);
	}

	_Tp size() const
	{
		return last - first;
	}

private:
	_Tp first;
	_Tp last;
};

#endif //TRANGE_H
