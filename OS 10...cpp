#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

int main() {
    HANDLE hPipe;
    char buffer[256];
    DWORD dwRead;
    
    printf("=== MESSAGE QUEUE SERVER ===\n");
    printf("Waiting for messages...\n\n");
    
    // Create a named pipe
    hPipe = CreateNamedPipeA(
        "\\\\.\\pipe\\MyMessageQueue",
        PIPE_ACCESS_INBOUND,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        1,           // Only one instance
        256,         // Output buffer size
        256,         // Input buffer size
        5000,        // Timeout in milliseconds
        NULL);       // Default security
    
    if (hPipe == INVALID_HANDLE_VALUE) {
        printf("Error creating pipe: %d\n", GetLastError());
        system("pause");
        return 1;
    }
    
    printf("Server ready. Pipe: \\\\.\\pipe\\MyMessageQueue\n");
    printf("Waiting for client connection...\n");
    
    // Wait for client to connect
    if (ConnectNamedPipe(hPipe, NULL)) {
        printf("\nClient connected!\n");
        
        // Read message from client
        if (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL)) {
            buffer[dwRead] = '\0';  // Null terminate
            printf("\n=== MESSAGE RECEIVED ===\n");
            printf("Content: %s\n", buffer);
            printf("Size: %d bytes\n", dwRead);
            printf("========================\n");
        }
    }
    
    printf("\nPress any key to close...");
    getchar();
    
    CloseHandle(hPipe);
    return 0;
}
