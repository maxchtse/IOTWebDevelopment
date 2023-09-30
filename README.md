### 1. Create a Virtual Environment:

To create a virtual environment named "myenv", you would use the following command:

```bash
python -m venv myenv
```

This creates a directory named "myenv" which contains a copy of the Python interpreter, the standard library, and various supporting files.

### 2. Activate the Virtual Environment:

After creating the virtual environment, you need to activate it. The activation command varies depending on your operating system:

- **Linux or macOS**:

  ```bash
  source myenv/bin/activate
  ```

- **Windows** (Command Prompt):

  ```bash
  myenv\Scripts\activate
  ```

- **Windows** (PowerShell):

  ```bash
  .\myenv\Scripts\Activate.ps1
  ```

Once the virtual environment is activated, your command prompt will change to show the name of the activated environment, and any Python packages you install using `pip` will be installed within this environment, isolated from the global Python installation.

### 3. Deactivate the Virtual Environment:

```bash
deactivate
```

### 4. Install Dependencies:

```bash
pip install -r requirements.txt
```

### 5. Run the Application:

```bash
flask run
```
