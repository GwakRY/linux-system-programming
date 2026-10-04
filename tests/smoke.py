"""Run built programs in disposable directories; Python stdlib only."""
from pathlib import Path
import re
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]


class SmokeTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.cwd = Path(self.temp.name)
        (self.cwd / "child").mkdir()
        (self.cwd / "sample.txt").write_text("sample\n")

    def run_program(self, binary, commands):
        result = subprocess.run([str(ROOT / binary)], input=commands,
                                text=True, capture_output=True, cwd=self.cwd,
                                timeout=5)
        self.assertEqual(result.returncode, 0, result.stderr)
        return re.sub(r"\x1b\[[0-9;]*m", "", result.stdout)

    def test_shell_multiple_commands_and_exit(self):
        output = self.run_program("small-shell/smallsh", "echo FIRST\necho SECOND\nexit\n")
        self.assertIn("FIRST", output)
        self.assertIn("SECOND", output)

    def test_shell_cd_relative_absolute_and_home(self):
        output = self.run_program("small-shell/smallsh",
            f"cd child\npwd\ncd {self.cwd}\npwd\ncd\npwd\ncd ~\npwd\n")
        self.assertIn(str(self.cwd / "child"), output)
        self.assertIn(str(Path.home()), output)

    def test_shell_semicolon_and_background(self):
        output = self.run_program("small-shell/smallsh", "echo ONE ; echo TWO\nsleep 0.1 &\nsleep 0.2\n")
        self.assertIn("ONE", output)
        self.assertIn("TWO", output)
        self.assertIn("[Process id]", output)

    def test_shell_bad_command_and_cd_recover(self):
        output = self.run_program("small-shell/smallsh", "command_does_not_exist_123\ncd missing\necho RECOVERED\n")
        self.assertIn("RECOVERED", output)

    def test_explorer_listing_and_details(self):
        output = self.run_program("directory-explorer/project1", "-2\n-2\n-1\n")
        self.assertIn("[X] sample.txt", output)
        self.assertIn("turn on the long list option", output)
        self.assertIn("turn off the long list option", output)
        self.assertRegex(output, r"-rw[-rwx]{7}")

    def test_explorer_directory_change(self):
        output = self.run_program("directory-explorer/project1", "3\n-1\n")
        self.assertIn(str(self.cwd / "child") + "$", output)

    def test_explorer_invalid_input_recovers(self):
        output = self.run_program("directory-explorer/project1", "0\n9999\nabc\n-1\n")
        self.assertEqual(output.count("invalid directory number"), 2)
        self.assertIn("Invalid input", output)

    def test_explorer_eof_and_long_name(self):
        name = "a" * 200
        (self.cwd / name).write_text("long name\n")
        output = self.run_program("directory-explorer/project1", "")
        self.assertIn(name, output)


if __name__ == "__main__":
    unittest.main(verbosity=2)
