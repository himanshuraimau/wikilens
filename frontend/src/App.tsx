import { useState } from "react";
import axios from "axios";
import { motion, AnimatePresence } from "framer-motion";
import "./index.css"

type Result = {
  title: string;
  score: number;
};

// Custom Styles for Glassmorphism
const glassStyle = {
  background: "rgba(255, 255, 255, 0.1)",
  backdropFilter: "blur(12px)",
  WebkitBackdropFilter: "blur(12px)",
  border: "1px solid rgba(255, 255, 255, 0.2)",
  boxShadow: "0 8px 32px 0 rgba(31, 38, 135, 0.15)",
};

function App() {
  const [query, setQuery] = useState("");
  const [results, setResults] = useState<Result[]>([]);
  const [loading, setLoading] = useState(false);
  const [searched, setSearched] = useState(false);

  const search = async () => {
    if (!query.trim()) return;
    setLoading(true);
    setSearched(true);

    try {
      const res = await axios.get(
        `http://localhost:8080/search?q=${encodeURIComponent(query)}`
      );
      setResults(res.data.results || []);
    } catch (err) {
      console.error("Search failed:", err);
    } finally {
      setLoading(false);
    }
  };

  return (
    <div style={{
      minHeight: "100vh",
      background: "linear-gradient(135deg, #0f172a 0%, #1e1b4b 50%, #312e81 100%)",
      color: "#f8fafc",
      padding: "40px 20px",
      fontFamily: "'Inter', system-ui, sans-serif"
    }}>
      <div style={{ maxWidth: "800px", margin: "0 auto" }}>
        
        {/* Header Section */}
        <header style={{ textAlign: "center", marginBottom: "60px" }}>
          <motion.h1 
            initial={{ opacity: 0, y: -20 }}
            animate={{ opacity: 1, y: 0 }}
            style={{
              fontSize: "4rem",
              fontWeight: "800",
              background: "linear-gradient(to right, #818cf8, #c084fc)",
              WebkitBackgroundClip: "text",
              WebkitTextFillColor: "transparent",
              margin: "0 0 10px 0",
              letterSpacing: "-0.05em"
            }}
          >
            WikiLens
          </motion.h1>
          <p style={{ color: "#94a3b8", fontSize: "1.1rem" }}>
            Precision search across documents via <span style={{ color: "#818cf8" }}>BM25 Ranking</span>
          </p>
        </header>

        {/* Search Bar Container */}
        <div style={{
          ...glassStyle,
          borderRadius: "24px",
          padding: "10px",
          display: "flex",
          gap: "10px",
          marginBottom: "50px",
          transition: "transform 0.2s ease"
        }}>
          <input
            value={query}
            onChange={(e) => setQuery(e.target.value)}
            onKeyDown={(e) => e.key === "Enter" && search()}
            placeholder="Search for anything..."
            style={{
              flex: 1,
              background: "transparent",
              border: "none",
              outline: "none",
              color: "white",
              padding: "15px 25px",
              fontSize: "1.1rem",
            }}
          />
          <button
            onClick={search}
            disabled={loading || !query.trim()}
            style={{
              background: "#6366f1",
              color: "white",
              border: "none",
              borderRadius: "16px",
              padding: "0 30px",
              fontWeight: "600",
              cursor: "pointer",
              transition: "all 0.3s ease",
              boxShadow: "0 4px 15px rgba(99, 102, 241, 0.4)"
            }}
          >
            {loading ? "..." : "Search"}
          </button>
        </div>

        {/* Results Area */}
        <div style={{ display: "flex", flexDirection: "column", gap: "20px" }}>
          <AnimatePresence>
            {results.map((result, index) => (
              <motion.div
                key={index}
                initial={{ opacity: 0, x: -20 }}
                animate={{ opacity: 1, x: 0 }}
                transition={{ delay: index * 0.05 }}
                whileHover={{ scale: 1.02, backgroundColor: "rgba(255, 255, 255, 0.15)" }}
                style={{
                  ...glassStyle,
                  borderRadius: "18px",
                  padding: "25px",
                  cursor: "pointer",
                }}
              >
                <div style={{ display: "flex", justifyContent: "space-between", alignItems: "center" }}>
                  <h3 style={{ margin: 0, fontSize: "1.25rem", color: "#e2e8f0" }}>
                    {result.title}
                  </h3>
                  <div style={{
                    fontSize: "0.8rem",
                    fontWeight: "bold",
                    color: "#818cf8",
                    background: "rgba(129, 140, 248, 0.1)",
                    padding: "4px 12px",
                    borderRadius: "20px",
                    border: "1px solid rgba(129, 140, 248, 0.3)"
                  }}>
                    SCORE: {result.score.toFixed(3)}
                  </div>
                </div>
              </motion.div>
            ))}
          </AnimatePresence>

          {/* Empty State */}
          {searched && !loading && results.length === 0 && (
            <motion.div 
              initial={{ opacity: 0 }}
              animate={{ opacity: 1 }}
              style={{ textAlign: "center", padding: "40px", color: "#64748b" }}
            >
              <div style={{ fontSize: "3rem", marginBottom: "10px" }}>🔭</div>
              <p>No documents found matching "<strong>{query}</strong>"</p>
            </motion.div>
          )}
        </div>
      </div>
    </div>
  );
}

export default App;