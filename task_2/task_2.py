import heapq
import os
from collections import defaultdict
from bs4 import BeautifulSoup
from PyPDF2 import PdfReader
from docx import Document

# A tree node
class Node:
    def __init__(self, char, freq):
        self.char = char
        self.freq = freq
        self.left = None
        self.right = None

    def __lt__(self, other):
        return self.freq < other.freq

# Function to generate Huffman codes
def generate_codes(node, prefix, codebook):
    if node is not None:
        if node.left is None and node.right is None:  # Leaf node
            codebook[node.char] = prefix
        generate_codes(node.left, prefix + '0', codebook)
        generate_codes(node.right, prefix + '1', codebook)

# Function to perform Huffman coding
def huffman_coding(text):
    # Calculate frequency of each character
    freq = defaultdict(int)
    for char in text:
        freq[char] += 1

    # Create a priority queue to store live nodes of the Huffman tree
    priority_queue = []
    for char, frequency in freq.items():
        heapq.heappush(priority_queue, Node(char, frequency))

    # Build the Huffman Tree
    while len(priority_queue) > 1:
        left = heapq.heappop(priority_queue)
        right = heapq.heappop(priority_queue)
        merged = Node(None, left.freq + right.freq)
        merged.left = left
        merged.right = right
        heapq.heappush(priority_queue, merged)

    # Generate codes
    codebook = {}
    generate_codes(priority_queue[0], '', codebook)

    return codebook

# Function to extract text from PDF file with error handling
def read_pdf(file_path):
    text = ""
    try:
        with open(file_path, "rb") as file:
            reader = PdfReader(file)
            for page in reader.pages:
                page_text = page.extract_text()
                if page_text:  # Check if page_text is not None
                    text += page_text + "\n"
    except PyPDF2.errors.PdfReadError as e:
        print("Error: The PDF file may be corrupted or not a valid PDF.")
    except Exception as e:
        print(f"Error reading PDF file: {e}")
    return text

# Function to extract text from DOCX file
def read_docx(file_path):
    doc = Document(file_path)
    text = ""
    for para in doc.paragraphs:
        text += para.text + "\n"
    return text

# Function to extract text from HTML file
def read_html(file_path):
    with open(file_path, "r", encoding="utf-8") as file:
        soup = BeautifulSoup(file, "html.parser")
        return soup.get_text(separator="\n")

# Function to read text from a .txt file
def read_txt(file_path):
    with open(file_path, "r", encoding="utf-8") as file:
        return file.read()

# Main function
def main():
    filename = input("Enter the file name (with extension): ")
    
    # Check file extension
    ext = os.path.splitext(filename)[1].lower()
    
    if ext == ".pdf":
        text = read_pdf(filename)
    elif ext == ".docx":
        text = read_docx(filename)
    elif ext == ".html":
        text = read_html(filename)
    elif ext == ".txt":
        text = read_txt(filename)
    else:
        print("Error: Unsupported file format. Please provide a PDF, DOCX, HTML, or TXT file.")
        return

    # Check if text is empty
    if not text.strip():
        print("Error: No valid content found in the file.")
        return

    # Remove non-printable characters
    sanitized_text = ''.join(filter(lambda x: x.isprintable(), text))

    # Generate Huffman codes
    huffman_codes = huffman_coding(sanitized_text)

    # Print Huffman codes
    print("Huffman Codes:")
    for char, code in huffman_codes.items():
        print(f"{repr(char)}: {code}")

    # Encode the input text
    encoded_text = ''.join(huffman_codes[char] for char in sanitized_text if char in huffman_codes)
    print(f"\nEncoded Text: {encoded_text}")

if __name__ == "__main__":
    main()
