import pygame

# 초기화
pygame.init()
pygame.joystick.init()

# 조이스틱 개수 확인
joystick_count = pygame.joystick.get_count()
print(f"Number of joysticks detected: {joystick_count}")

if joystick_count == 0:
    print("No joystick found.")
    pygame.quit()
    exit()

# 첫 번째 조이스틱 선택
joystick = pygame.joystick.Joystick(0)
joystick.init()

print(f"Joystick Name: {joystick.get_name()}")
print(f"Number of Axes: {joystick.get_numaxes()}")
print(f"Number of Buttons: {joystick.get_numbuttons()}")

# 무한 루프에서 조이스틱 데이터 읽기
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.JOYAXISMOTION:
            axis_data = [joystick.get_axis(i) for i in range(joystick.get_numaxes())]
            print("Axis:", axis_data)
        elif event.type == pygame.JOYBUTTONDOWN:
            button_data = [joystick.get_button(i) for i in range(joystick.get_numbuttons())]
            print("Buttons:", button_data)
        elif event.type == pygame.QUIT:
            running = False

# 종료
pygame.quit()