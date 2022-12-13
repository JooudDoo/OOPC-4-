#include <iostream>
#include <string>
#include <tuple>

#if _HAS_CXX17
// Использует свертку. С17
namespace tuplePrinterUtils {

	template <class Ch, class Tr, class T>
	static void printElement___(std::basic_ostream<Ch, Tr>& os, const T& elem) {
		os << elem << ", ";
	}

	template<class Ch, class Tr, class TTuple, size_t... i>
	static void printElements___(std::basic_ostream<Ch, Tr>& os, TTuple const& t, std::index_sequence<i...>) {
		(printElement___(os, std::get<i>(t)), ...);
	}

	template<class Ch, class Tr, class... Args>
	static void printLast___(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t) {
		const size_t index = sizeof...(Args) - 1;
		os << std::get<index>(t);
	}

}

template<class Ch, class Tr, class... Args>
auto& tuplePrinter(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t) {
	os << "(";
	tuplePrinterUtils::printElements___(os, t, std::make_index_sequence<sizeof...(Args)-1>());
	tuplePrinterUtils::printLast___(os, t);
	os << ")";
	return os;
}

#else
namespace tuplePrinterUtils {
	template<class Ch, class Tr, size_t A, class... Args>
	struct tuplePrinter___ {
		static void print___(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t) {
			const size_t index = sizeof...(Args) - A;
			os << std::get<index>(t);
			if (A > 1) os << ", ";
			tuplePrinter___<Ch, Tr, A - 1, Args...>::print___(os, t);
		}
	};

	template<class Ch, class Tr, class... Args>
	struct tuplePrinter___<Ch, Tr, 0, Args...> {
		static void print___(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t) {};
	};
}

template<class Ch, class Tr, class... Args>
auto& tuplePrinter(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t) {
	os << "(";
	tuplePrinterUtils::tuplePrinter___<Ch, Tr, sizeof...(Args), Args...>::print___(os, t);
	os << ")";
	return os;
}
#endif

template<class Ch, class Tr, class... Args>
auto& operator<<(std::basic_ostream<Ch, Tr>& os, std::tuple<Args...> const& t) {
	return tuplePrinter<Ch, Tr, Args...>(os, t);
}

int main() {
	std::tuple<int, char, std::string, double> a = { 1, 'a', "da", 2 };
	std::cout << a;
}