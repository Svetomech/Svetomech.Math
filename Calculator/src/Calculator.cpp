#include <iostream>
#include <fstream>
#include <string>
#include <stack>

class CalculationNotDoneException: public std::exception
{
    virtual const char* what() const throw()
    {
        return "The Input() method should be executed before Output()";
    }
} calc_not_done;

class IncorrectOperatorException: public std::exception
{
    virtual const char* what() const throw()
    {
        return "Allowed operators are: +,-,*,/,^";
    }
} wrong_sign;

class EmptyInputException: public std::exception
{
    virtual const char* what() const throw()
    {
        return "The input string must not be empty";
    }
} no_input;

enum class InputType
{
    Console,
    File,
    Constructor
};

class Calculator
{
    public:
        std::string InputFileName;
        std::string OutputFileName;

        Calculator() :
            InputFileName("Input.txt"),
            OutputFileName("Output.txt"),
            result(0),
            calculationDone(false)
        {
            // empty constructor, totally legit
        }

        Calculator(const InputType& inputFrom)
        {
            Calculator();

            Input(inputFrom);
        }

        Calculator(const InputType& inputFrom, const std::string& expression)
        {
            Calculator();

            Input(inputFrom, expression);
        }

        Calculator(const InputType& inputFrom, const std::string& inputFileName, const std::string& outputFileName)
        {
            Calculator();

            InputFileName = inputFileName;
            OutputFileName = outputFileName;

            Input(inputFrom);
        }

        void Input(const InputType& inputFrom, const std::string& expression = "")
        {
            inputType = inputFrom;

            if (InputType::Console == inputType)
            {
                std::getline(std::cin, input);
            }
            else if (InputType::File == inputType)
            {
                std::ifstream inputFile(InputFileName);

                if (inputFile.is_open())
                {
                    std::getline(inputFile, input);

                    inputFile.close();
                }
                else
                {
                    std::cout << "Can't read from file named " << InputFileName << std::endl;

                    return;
                }
            }
            else if (InputType::Constructor == inputType)
            {
                input = expression;
            }

            processInput();
        }

        double Output()
        {
            if (!calculationDone) throw calc_not_done;

            if (InputType::Console == inputType)
            {
                std::cout << result << std::endl;
            }
            else if (InputType::File == inputType)
            {
                std::ofstream outputFile(OutputFileName);

                if (outputFile.is_open())
                {
                    outputFile << result << std::endl;

                    outputFile.close();
                }
                else
                {
                    std::cout << "Can't create a file named " << OutputFileName << std::endl;

                    return result;
                }
            }
            else if (InputType::Constructor == inputType)
            {
                // nothing specific, just return the result
            }

            calculationDone = false;

            return result;
        }

    private:
        double result;
        bool calculationDone;
        InputType inputType;
        std::string input;

        // TODO
        void processInput()
        {
            if (input.empty()) throw no_input;

            result = calculateOutput(input);

            std::stack<std::string> st;
            std::string out;

            for (int i = 0; i < input.length(); ++i)
            {
                char c = input[i];
                char cn = input[i + 1];


            }
        }

        /*void processInputOld()
        {
            std::stack<char> st;
            std::string out;

            for (int i = 0; i < input.length(); ++i)
            {
                char c = input[i];

                if (c != ' ')
                {
                    if (c == ')')
                    {
                        while (st.top() != '(')
                        {
                            out += st.top();
                            st.pop();
                        }
                        st.pop();
                    }

                    if (isNumOrDot(c))
                    {
                        out += c;
                    }

                    if (c == '(')
                    {
                        st.push(c);
                    }

                    if (isOperator(c))
                    {
                        if (st.empty())
                        {
                            st.push(c);
                        }
                        else
                        {
                            if (operationPriority(st.top()) < operationPriority(c))
                            {
                                st.push(c);
                            }
                            else
                            {
                                while (!st.empty() && (operationPriority(st.top()) >= operationPriority(c)))
                                {
                                    out += st.top();
                                    st.pop();
                                }

                                st.push(c);
                            }
                        }
                    }
                }
                else
                {
                    st.push(c);
                }
            }

            std::cout << "RPN1: " << out << std::endl;

            while (!st.empty())
            {
                out += st.top();
                st.pop();
            }

            std::cout << "RPN2: " << out << std::endl;

            // result = calculateOutput(out);
        }*/

        bool isNumOrDot(char c)
        {
            return ((c >= '0' && c <= '9') || c == '.');
        }

        bool isOperator(char c)
        {
            return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
        }

        int operationPriority(char sign)
        {
            switch(sign)
            {
                case '^':
                case ' ':
                    return 4;

                case '/':
                case '*':
                    return 3;

                case '-':
                case '+':
                    return 2;

                case '(':
                    return 1;

                default:
                    throw wrong_sign;
            }
        }

        double makePower(double number, int powerOf)
        {
            double baseNumber = number;

            for (int i = 1; i < powerOf; ++i)
            {
                number *= baseNumber;
            }

            return number;
        }

        double calculateOutput(const std::string& rpnString)
        {
            std::stack<std::string> st;

            for (int i = 0; i < rpnString.length(); ++i)
            {
                char c = rpnString[i]; // std::cout << "c: " << c << std::endl;
                char cn = rpnString[i + 1]; // std::cout << "cn: " << cn << std::endl;

                if (isOperator(c) && !isNumOrDot(cn))
                {
                    double a = std::stod(st.top()); // std::cout << "a: " << a;
                    st.pop();
                    double b = std::stod(st.top()); // std::cout << "b: " << b << std::endl;
                    st.pop();

                    switch(c)
                    {
                        case '+':
                            st.push(std::to_string(b + a));
                            break;

                        case '-':
                            st.push(std::to_string(b - a));
                            break;

                        case '*':
                            st.push(std::to_string(b * a));
                            break;

                        case '/':
                            st.push(std::to_string(b / a));
                            break;

                        case '^':
                            st.push(std::to_string(makePower(b, a)));
                            break;
                    }
                }
                else if (isNumOrDot(c) || (isOperator(c) && isNumOrDot(cn)))
                {
                    std::string s(1, c);

                    while (isNumOrDot(cn = rpnString[++i]))
                    {
                        s += cn;
                    }

                    st.push(s); // std::cout << "pushed s: " << s << std::endl;
                }
            }

            calculationDone = true;

            return std::stod(st.top());
        }
};

int main()
{
    // Use case #1 - expression comes from console
    auto calc1 = new Calculator(InputType::Console); // Input() method called automatically
    calc1->Output();

    // Use case #2 - expression comes from file
    auto calc2 = new Calculator(InputType::File, "test.in", "test.out"); // custom filenames
    calc2->Output();

    // Use case #3 - expression in a constructor
    auto calc3 = new Calculator(InputType::Constructor, "(9+1)*(1+1)-5"); // brackets supported
    calc3->Output();
}
