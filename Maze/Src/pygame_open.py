import sys, os 
import pygame
import subprocess

def initialize():
    pygame.init()
    clock = pygame.time.Clock()
    return clock 

def bringGameData():
    # Set up the Visual Studio environment variables
    file_name = "temptest.cpp"
    vcvars_path = "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Auxiliary\\Build\\vcvars64.bat"
    subprocess.run([vcvars_path, "&&", "cl", "/EHsc", "temptest.cpp", "/link", "/ENTRY:main_test", "main_test.cpp"], check=True, shell=True)

    # Run the generated executable and capture the output
    result = subprocess.run([file_name], stdout=subprocess.PIPE, text=True)
    print("C++ output:")

    # (Operation).cpp -> (Play).py then you don't need to leave those expansion files.
    os.remove("temptest.exe")
    os.remove("temptest.obj")
    return 

def setupDisplay(width : int, height: int ):
    screen = pygame.display.set_mode((width, height))
    pygame.display.set_caption('C++ Maze but in pygame')
    return screen

def runAndShow(clock, screen):
    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
    
    # fill the screen with a color to wipe away anything from last frame
    screen.fill("purple")
    # flip() the display to put your work on screen
    pygame.display.flip()
    clock.tick(60)
    

def quitGame():
    return 

def mainRun():
        
    clock = initialize()
    bringGameData()
    screen = setupDisplay(1280, 720)
    runAndShow(clock, screen)
    
    
    return

if __name__ == "__main__": 
    mainRun()
    pygame.quit()
