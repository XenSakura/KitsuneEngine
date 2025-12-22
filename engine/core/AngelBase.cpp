
import ExceptionHandler;

int main()
{
    try_catch([]()
    {
        int * i = nullptr;
       int j = *i; 
    });
    
}
