import heapq
from collections import Counter
import pdfplumber
from docx import Document
from bs4 import BeautifulSoup
import os

# Huffman Node class for building the Huffman Tree
class Node:
    def __init__(self, char, freq):  # Corrected constructor
        self.char = char
        self.freq = freq
        self.left = None
        self.right = None

    def __lt__(self, other):  # Corrected comparator
        return self.freq < other.freq

# Huffman Coding class with .dot file generation
class HuffmanCoding:
    def __init__(self):  # Corrected constructor
        self.codes = {}
        self.reverse_mapping = {}

    def calculate_frequencies(self, text):
        return Counter(text)

    def build_heap(self, frequency):
        heap = []
        for char, freq in frequency.items():
            node = Node(char, freq)
            heapq.heappush(heap, node)
        return heap

    def merge_nodes(self, heap):
        while len(heap) > 1:
            node1 = heapq.heappop(heap)
            node2 = heapq.heappop(heap)
            merged = Node(None, node1.freq + node2.freq)
            merged.left = node1
            merged.right = node2
            heapq.heappush(heap, merged)
        return heap[0]

    def build_codes_helper(self, root, current_code):
        if root is None:
            return
        if root.char is not None:
            self.codes[root.char] = current_code
            self.reverse_mapping[current_code] = root.char
            return
        self.build_codes_helper(root.left, current_code + "0")
        self.build_codes_helper(root.right, current_code + "1")

    def build_codes(self, root):
        self.build_codes_helper(root, "")

    def get_encoded_text(self, text):
        encoded_text = "".join([self.codes[char] for char in text])
        return encoded_text

    def huffman_compress(self, text):
        frequency = self.calculate_frequencies(text)
        heap = self.build_heap(frequency)
        root = self.merge_nodes(heap)
        self.build_codes(root)
        encoded_text = self.get_encoded_text(text)
        self.generate_dot_file(root)  # Generate .dot file for Huffman Tree visualization
        return encoded_text

    def generate_dot_file(self, root, filename="huffman_tree.dot"):
        """Generate a .dot file for the Huffman Tree"""
        with open(filename, "w") as file:
            file.write("digraph G {\n")
            self.write_dot_nodes(file, root)
            file.write("}\n")
        print(f"Huffman Tree saved as {filename}")

    def write_dot_nodes(self, file, node, parent_id=""):
        """Helper function to write nodes in .dot format"""
        if node is not None:
            node_id = id(node)
            label = f'"{node.char}:{node.freq}"' if node.char else f'"{node.freq}"'
            file.write(f'  {node_id} [label={label}];\n')
            if parent_id:
                file.write(f'  {parent_id} -> {node_id};\n')
            self.write_dot_nodes(file, node.left, str(node_id))
            self.write_dot_nodes(file, node.right, str(node_id))

    def read_file(self, filename):
        """Detect file type and extract text accordingly."""
        _, file_extension = os.path.splitext(filename)
        text = ""

        if file_extension.lower() == ".pdf":
            text = self.read_pdf(filename)
        elif file_extension.lower() == ".docx":
            text = self.read_docx(filename)
        elif file_extension.lower() == ".html":
            text = self.read_html(filename)
        elif file_extension.lower() == ".txt":
            text = self.read_txt(filename)
        else:
            raise ValueError("Unsupported file format!")
        
        return text

    def read_pdf(self, filepath):
        """Extract text from PDF file."""
        text = ""
        with pdfplumber.open(filepath) as pdf:
            for page in pdf.pages:
                text += page.extract_text() + "\n"
        return text

    def read_docx(self, filepath):
        """Extract text from DOCX file."""
        doc = Document(filepath)
        text = "\n".join([para.text for para in doc.paragraphs])
        return text

    def read_html(self, filepath):
        """Extract text from HTML file."""
        with open(filepath, "r", encoding="utf-8") as file:
            soup = BeautifulSoup(file, "html.parser")
            text = soup.get_text()
        return text

    def read_txt(self, filepath):
        """Extract text from TXT file."""
        with open(filepath, "r", encoding="utf-8") as file:
            text = file.read()
        return text

# Test the HuffmanCoding with a file
filename = "test_case_3.docx"  # Replace with your file path
huffman = HuffmanCoding()
text = huffman.read_file(filename)
encoded_text = huffman.huffman_compress(text)

print("Original Text:", text[:100])  # Printing only first 100 chars for brevity
print("Encoded Text:", encoded_text[:100])  # Printing only first 100 chars for brevity
