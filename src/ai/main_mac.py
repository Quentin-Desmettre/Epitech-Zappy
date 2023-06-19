#!/opt/homebrew/bin/python3

from src.ai.main import main
from src.ai.utils import my_print, set_color, Colors

if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        set_color(Colors.FAIL)
        my_print(e)
    except KeyboardInterrupt:
        my_print("Interrupted")
    except SystemExit:
        my_print("Exiting...")
