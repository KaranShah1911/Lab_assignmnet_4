import heapq
from collections import Counter
import os
import pdfplumber
import docx

# Huffman Node class for building the Huffman Tree
class Node:
    def __init__(self, char, freq):  # Constructor
        self.char = char
        self.freq = freq
        self.left = None
        self.right = None

    def __lt__(self, other):  # Comparator
        return self.freq < other.freq

# Huffman Coding class with .dot file generation and validation checks
class HuffmanCoding:
    def __init__(self):  # Constructor
        self.codes = {}
        self.reverse_mapping = {}

    def read_file(self, filepath):
        """Reads content from various file types and detects negative cases."""
        if not os.path.exists(filepath):
            print("Invalid input: File does not exist.")
            return None
        
        if filepath.endswith('.txt'):
            return self.read_txt(filepath)
        elif filepath.endswith('.pdf'):
            return self.read_pdf(filepath)
        elif filepath.endswith('.docx'):
            return self.read_docx(filepath)
        else:
            print("Invalid input: Unsupported file type.")
            return None

    def read_txt(self, filepath):
        with open(filepath, 'r', encoding='utf-8') as file:
            text = file.read()
        return self.validate_content(text)

    def read_pdf(self, filepath):
        try:
            with pdfplumber.open(filepath) as pdf:
                text = ''.join(page.extract_text() for page in pdf.pages if page.extract_text())
            return self.validate_content(text)
        except Exception:
            print("Invalid input: Unable to read or corrupted PDF file.")
            return None

    def read_docx(self, filepath):
        try:
            doc = docx.Document(filepath)
            text = ''.join(paragraph.text for paragraph in doc.paragraphs)
            return self.validate_content(text)
        except Exception:
            print("Invalid input: Unable to read or corrupted DOCX file.")
            return None

    def validate_content(self, text):
        """Validates if content is non-empty and has only supported characters."""
        if not text:
            print("Invalid input: Empty file.")
            return None

        if not all(32 <= ord(char) <= 126 for char in text):
            print("Invalid input: Unsupported characters in file.")
            return None

        return text

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
        return "".join([self.codes[char] for char in text])

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

# Test the HuffmanCoding with a file input
filename = "corrupted_file.pdf"  # Change this to test different files
huffman = HuffmanCoding()
text = huffman.read_file(filename)

if text is not None:
    encoded_text = huffman.huffman_compress(text)
    print("Encoded Text:", encoded_text)
else:
    print("No encoding performed due to invalid input.")
