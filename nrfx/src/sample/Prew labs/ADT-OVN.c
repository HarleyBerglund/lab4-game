#include "goodFuncs.h"
#include <nrfx_systick.h>

#define CLEAR_SCREEN "\033c"

typedef struct employee{
    uint16_t name[100];
    uint8_t yearOfBirth;
    uint64_t sallary;

}Employee;

//ADT
// Employee createEmployee(uint16_t name, uint8_t yearOfBirth);
// pre: Det ska finnas ett namn(string) och ett födelseår(int)
// post: Det ska skapas en ny anställd 

// void addSallary(Employee *employee, uint8_t sallaryIncrease);
// pre: det ska finnas en pekare till en employee och en löneökning(int)
// post: Employeens lön kommer öka med sallaryincrease

// void printEmployee(Employee employeeToPrint);
// pre: du ska ha en employee att skrivas ut
// post: en employee kommer skickas med uarte



Employee createEmployee(uint16_t name, uint8_t yearOfBirth){
    Employee tempEmployee;
    tempEmployee.name = name;
    tempEmployee.yearOfBirth = yearOfBirth;

    return tempEmployee;
};

void addSallary(Employee *employee, uint8_t sallaryIncrease){

    employee.sallary += sallaryIncrease;
};

void printEmployee(Employee employeeToPrint){

    char nameBuffer[100] = "Name: ";
    strcat(nameBuffer, employeeToPrint.name, strlen(employeeToPrint.name));

    char yearBuffer[100] = "Year of Birth: ";
    strcat(yearBuffer, sprintf(employeeToPrint.yearOfBirth), strlen(sprintf(employeeToPrint.yearOfBirth)));

    char sallaryBuffer[100] = "Sallary: ";
    strcat(sallaryBuffer, sprintf(employeeToPrint.sallary), strlen(sprintf(employeeToPrint.sallary)));
    

    send_string(nameBuffer);
    send_string(yearBuffer);
    send_string(sallaryBuffer);
    
};


int main(void) {
    int8_t clearscreen[] = CLEAR_SCREEN; 
    // Use default settings and specify our TX and RX pins:
    const nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(PIN_TXD, PIN_RXD);

    // Initialize UARTE:
    nrfx_err_t errr = nrfx_uarte_init(&instance, &config, NULL);
    if (errr != 0) {
        // Implement error handling here, but for now, let's hope everything works
    }
    nrfx_systick_init();

    uarte_write(clearscreen, sizeof(clearscreen));
    Employee employeeList[];
    while(1){
        uint8_t ledMsg[] = "\rWhat do you want to do: \r\n \r1: Add new employee \r\n \r2: Increase sallary\r\n \r3: Print employee\r\n ";
        uarte_write(ledMsg, sizeof(ledMsg));  
        int choice = read_int();
        uarte_write(clearscreen, sizeof(clearscreen));

        switch (choice){
            case 1:
                Employee newEmployee;
            
                nameBuffer[100];
                send_string("Please enter name of the employee:\n");
                read_string(nameBuffer);
                uarte_write(clearscreen, sizeof(clearscreen));

                send_string("Please enter the birth year of the employee:\n");
                int yearOfEmployee = read_int();
                uarte_write(clearscreen, sizeof(clearscreen));

                newEmployee = createEmployee(nameBuffer, yearOfEmployee);
                newEmployee 
                break;
            case 2:
                addSallary();
                break;
            case 3:
                nameBuffer[100];
                send_string("Please enter name of the empoyee to print:\n");
                read_string(nameBuffer);
                uarte_write(clearscreen, sizeof(clearscreen));

                printEmployee();
                break;
        }
        uarte_write(clearscreen, sizeof(clearscreen));

    }
    return 0;
}