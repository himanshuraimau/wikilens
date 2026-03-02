import { useState } from "react";
import axios from "axios";

type Result = {
  title: string;
  score: number;
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
      setResults(res.data.results);
    } catch (err) {
      console.error(err);
    }

    setLoading(false);
  };

  const handleKeyPress = (e: React.KeyboardEvent) => {
    if (e.key === "Enter") {
      search();
    }
  };

  return (
    <div style={{
      minHeight: "100vh",
      background: "linear-gradient(135deg, #667eea 0%, #764ba2 100%)",
      padding: "20px",
      fontFamily: "-apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, sans-serif"
    }}>
      <div style={{
        width: "1400px",
        margin: "0 auto",
        paddingTop: "60px"
      }}>
        {/* Header */}
        <div style={{ textAlign: "center", marginBottom: "40px" }}>
          <h1 style={{
            fontSize: "3.5rem",
            color: "white",
            marginBottom: "10px",
            fontWeight: "700",
            letterSpacing: "-0.5px"
          }}>
            WikiLens 🔍
          </h1>
          <p style={{
            color: "rgba(255, 255, 255, 0.9)",
            fontSize: "1.1rem",
            margin: 0
          }}>
            Intelligent document search powered by BM25
          </p>
        </div>

        {/* Search Box */}
        <div style={{
          background: "white",
          borderRadius: "12px",
          padding: "8px",
          boxShadow: "0 10px 40px rgba(0, 0, 0, 0.2)",
          marginBottom: "40px",
          display: "flex",
          gap: "8px"
        }}>
          <input
            value={query}
            onChange={(e) => setQuery(e.target.value)}
            onKeyPress={handleKeyPress}
            placeholder="Search documents..."
            style={{
              flex: 1,
              padding: "14px 20px",
              border: "none",
              outline: "none",
              fontSize: "16px",
              borderRadius: "8px",
              fontFamily: "inherit"
            }}
          />
          <button
            onClick={search}
            disabled={loading || !query.trim()}
            style={{
              padding: "14px 32px",
              background: loading ? "#9ca3af" : "#667eea",
              color: "white",
              border: "none",
              borderRadius: "8px",
              fontSize: "16px",
              fontWeight: "600",
              cursor: loading || !query.trim() ? "not-allowed" : "pointer",
              transition: "all 0.2s",
              whiteSpace: "nowrap"
            }}
            onMouseEnter={(e) => {
              if (!loading && query.trim()) {
                e.currentTarget.style.background = "#5568d3";
              }
            }}
            onMouseLeave={(e) => {
              if (!loading) {
                e.currentTarget.style.background = "#667eea";
              }
            }}
          >
            {loading ? "Searching..." : "Search"}
          </button>
        </div>

        {/* Results */}
        {searched && !loading && results.length === 0 && (
          <div style={{
            background: "rgba(255, 255, 255, 0.95)",
            borderRadius: "12px",
            padding: "40px",
            textAlign: "center",
            color: "#6b7280"
          }}>
            <p style={{ fontSize: "1.1rem", margin: 0 }}>
              No results found for "{query}"
            </p>
          </div>
        )}

        {results.length > 0 && (
          <div style={{
            display: "flex",
            flexDirection: "column",
            gap: "16px"
          }}>
            {results.map((r, i) => (
              <div
                key={i}
                style={{
                  background: "rgba(255, 255, 255, 0.95)",
                  borderRadius: "12px",
                  padding: "24px",
                  boxShadow: "0 4px 6px rgba(0, 0, 0, 0.1)",
                  transition: "all 0.2s",
                  cursor: "pointer"
                }}
                onMouseEnter={(e) => {
                  e.currentTarget.style.transform = "translateY(-2px)";
                  e.currentTarget.style.boxShadow = "0 8px 12px rgba(0, 0, 0, 0.15)";
                }}
                onMouseLeave={(e) => {
                  e.currentTarget.style.transform = "translateY(0)";
                  e.currentTarget.style.boxShadow = "0 4px 6px rgba(0, 0, 0, 0.1)";
                }}
              >
                <div style={{
                  display: "flex",
                  justifyContent: "space-between",
                  alignItems: "start",
                  gap: "16px",
                  flexWrap: "wrap"
                }}>
                  <h3 style={{
                    margin: 0,
                    fontSize: "1.25rem",
                    color: "#1f2937",
                    fontWeight: "600",
                    flex: 1
                  }}>
                    {r.title}
                  </h3>
                  <div style={{
                    background: "#f3f4f6",
                    padding: "6px 12px",
                    borderRadius: "6px",
                    fontSize: "0.875rem",
                    color: "#6b7280",
                    fontWeight: "500"
                  }}>
                    Score: {r.score.toFixed(4)}
                  </div>
                </div>
              </div>
            ))}
          </div>
        )}
      </div>
    </div>
  );
}

export default App;